//
// Created by Tobias on 19.04.2025.
//

#ifndef IDMANAGER_H
#define IDMANAGER_H


class IdManager {
public:
    unsigned int generate_id();

private:
    unsigned int id = 0;
};


#endif //IDMANAGER_H
