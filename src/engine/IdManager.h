//
// Created by Tobias on 19.04.2025.
//

#ifndef IDMANAGER_H
#define IDMANAGER_H


class IdManager {
public:
    static unsigned int gen_id();

private:
    inline static unsigned int id = 0;
};


#endif //IDMANAGER_H
