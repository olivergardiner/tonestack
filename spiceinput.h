#ifndef SPICEINPUT_H
#define SPICEINPUT_H
#include "ngspice/sharedspice.h"
#include <QFile>

#include <QFileInfo>
#include <QDir>
#include <QProcess>
#include <QSet>
#include <functional>

// Read include files ourselves: the shared-library command parser does not
// unquote filenames consistently, and changing cwd would race the GUI thread.
inline int loadSpiceFile(const QString &path, QString *error = nullptr)
{
    QList<QByteArray> lines;
    QSet<QString> stack;
    QString failure;
    std::function<bool(QString,int)> append = [&](QString filename, int depth) {
        filename=QFileInfo(filename).absoluteFilePath();
        if (depth>32 || stack.contains(filename)) {
            failure="Recursive or excessively deep SPICE include: "+filename; return false;
        }
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) { failure=filename+": "+file.errorString(); return false; }
        stack.insert(filename);
        for (auto line : file.readAll().split('\n')) {
            if (line.endsWith('\r')) line.chop(1);
            const auto parts=QProcess::splitCommand(QString::fromLocal8Bit(line).trimmed());
            if (!parts.isEmpty() && (parts[0].compare(".include",Qt::CaseInsensitive)==0 ||
                                     parts[0].compare(".inc",Qt::CaseInsensitive)==0)) {
                if (parts.size()!=2) { failure="Invalid include in "+filename; return false; }
                const QString included=QDir(QFileInfo(filename).absolutePath()).absoluteFilePath(parts[1]);
                if (!append(included,depth+1)) return false;
            } else {
                if (parts.size()>=3 && parts[0].compare(".lib",Qt::CaseInsensitive)==0) {
                    const auto library=QDir(QFileInfo(filename).absolutePath()).absoluteFilePath(parts[1]);
                    line=(".lib \"" + QDir::fromNativeSeparators(library) + "\" " + parts.mid(2).join(' ')).toLocal8Bit();
                }
                lines.append(line);
            }
        }
        stack.remove(filename);
        return true;
    };
    if (!append(path,0)) { if(error) *error=failure; return 1; }
    QVector<char*> pointers; bool hasEnd = false;
    for (auto &line : lines) {
        // The shared-library array interface recognizes a literal .end marker.
        if (line.trimmed().toLower() == ".end") { line = ".end"; hasEnd = true; }
        if (!line.trimmed().isEmpty()) pointers.append(line.data());
        if (hasEnd) break;
    }
    if (!hasEnd) { if (error) *error="Missing .end in "+path; return 1; }
    QByteArray terminator(1, '\0'); // Older ngspice requires an empty card after .end.
    pointers.append(terminator.data());
    pointers.append(nullptr);
    return ngSpice_Circ(pointers.data());
}
#endif
