#ifndef ASSETPATHS_H
#define ASSETPATHS_H
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>
#include <QProcess>

namespace AssetPaths {
inline QString circuitsDirectory()
{
    QDir app(QCoreApplication::applicationDirPath());
    if (QFileInfo::exists(app.filePath("circuits/tonestacks.tsc")))
        return app.filePath("circuits");
    // Compatibility with the original installer: bin/ beside circuits/.
    return QDir::cleanPath(app.filePath("../circuits"));
}
inline QString sourcePath(const QString &command)
{
    const auto parts = QProcess::splitCommand(command);
    if (parts.size()==2 && parts[0].compare("source",Qt::CaseInsensitive)==0)
        return parts[1];
    // Legacy catalogues did not quote paths.
    if (command.startsWith("source ",Qt::CaseInsensitive))
        return command.mid(7).trimmed();
    return {};
}
inline QString sourceCommand(const QString &path)
{
    const auto clean=QDir::fromNativeSeparators(path);
    if (!clean.contains(' ') && !clean.contains('\t')) return "source " + clean;
    return "source \"" + clean + "\"";
}
inline QString resolve(const QString &path, const QString &base = QString())
{
    if (path.isEmpty() || QFileInfo(path).isAbsolute()) return path;
    const QString clean = QDir::fromNativeSeparators(path);
    // Historical bundled references are portable across installations. Prefer a
    // real document-local file first so custom libraries can retain this layout.
    if (!base.isEmpty() && QFileInfo::exists(QDir(base).filePath(clean)))
        return QDir::cleanPath(QDir(base).absoluteFilePath(clean));
    for (const auto prefix : {"../circuits/","circuits/"})
        if (clean.startsWith(prefix))
            return QDir::cleanPath(QDir(circuitsDirectory()).filePath(clean.mid(QString(prefix).size())));
    return QDir::cleanPath(QDir(base.isEmpty() ? circuitsDirectory() : base).absoluteFilePath(clean));
}
inline QJsonObject resolved(QJsonObject object, const QString &base = QString())
{
    const auto path = sourcePath(object.value("command").toString());
    if (!path.isEmpty()) object["command"]=sourceCommand(resolve(path,base));
    if (object.contains("image")) object["image"]=resolve(object["image"].toString(),base);
    return object;
}
inline QString portable(const QString &path, const QString &destination)
{
    const auto relative = QDir(circuitsDirectory()).relativeFilePath(path);
    if (!QFileInfo(relative).isAbsolute() && relative != ".." && !relative.startsWith("../"))
        return "../circuits/" + relative;
    return QDir(destination).relativeFilePath(path);
}
inline QJsonObject forSave(QJsonObject object, const QString &destination)
{
    object=resolved(object);
    const auto source=sourcePath(object["command"].toString());
    if (!source.isEmpty()) object["command"]=sourceCommand(portable(source,destination));
    if (object.contains("image") && !object["image"].toString().isEmpty())
        object["image"]=portable(object["image"].toString(),destination);
    return object;
}
}
#endif
