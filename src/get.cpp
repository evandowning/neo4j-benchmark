#include <stdlib.h>
#include "get.h"

#include "first.h"
#include "second.h"
#include "third.h"
#include "fourth.h"
#include "batch.h"
#include "unwind.h"
#include "loadcsv.h"

struct ALG *getAlg(int c){
    switch(c) {
        case 0:
            return &getFirst;
        case 1:
            return &getSecond;
        case 2:
            return &getThird;
        case 3:
            return &getFourth;
        case 4:
            return &getBatch;
        case 5:
            return &getUnwind;
        case 6:
            return &getLoadcsv;
        default:
            return NULL;
    }
}
