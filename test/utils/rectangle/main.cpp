#include "gtest/gtest.h"

// Tova e vxodnata tochka na Google Framework-a kum nasheto prilozhenie


// every binary MUST have a main.cpp function
int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); // tazi funkcia vzima vsichki testove koito sme napisali v nasheto binnary i gi execute-va
}

// taka che main-a koito imame, v 90% ot sluchaite shte izglezhda taka...
// ... ili nai-mnogo da printnem nqkoi red - koi test se runva ili neshto ot sorta





// ASIDE: Pri uspehsno build-vane i compilirane, testa / testovete sa gotovi za runvane. 
// Zhivko kazva che ima nqkakvo macro s koeto se run-vat vsicki testove, no ne e mogul da go nameri
// Vse pak mozhe da se run-nat samite testove - vlizame kudeto se namirat