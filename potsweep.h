#ifndef POTSWEEP_H
#define POTSWEEP_H

enum potType {
    POT_LIN,
    POT_LOG,
    POT_LOGA,
    POT_RLOG,
    POT_STYPE
};

class PotSweep
{
public:
    virtual double taperFunction(int potType, double sweep) = 0;
};

#endif // POTSWEEP_H
