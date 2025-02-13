// version.h
#ifndef VERSION_H
#define VERSION_H

struct Version {
    int current;
    int last;
};

Version readVersionFile();
void updateLastVersion(int version);

#endif