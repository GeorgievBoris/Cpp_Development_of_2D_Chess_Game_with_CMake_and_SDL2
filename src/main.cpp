// C system includes
#include <cstdint>
#include <cstdlib>
// C++ system includes
#include <iostream>
// Third-party includes
// Own includes
#include "engine/Engine.h"
#include "sdl_utils/SDLLoader.h"
#include "engine/EngineCfgLoader.h"

static int32_t runApplication() {
    Engine engine;

    if(EXIT_SUCCESS!=engine.init(EngineCfgLoader::loadCfg())){
        std::cerr<<"engine.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    engine.start();

    engine.deinit();

    return EXIT_SUCCESS;
}

int32_t main([[maybe_unused]]int32_t argc, [[maybe_unused]]char* argv[]){
// int32_t main (int32_t argc, char* argv[]){
//     (void)argc; (void)argv;

    if(EXIT_SUCCESS!=SDLLoader::init()){ // this is an initialization of any dependencies (i.e. the SDL libary)
        std::cerr<<"SDLLoader::init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=runApplication()){
        std::cerr<<"runApplication() failed."<<std::endl;
        return EXIT_FAILURE;
    }

    SDLLoader::deinit(); // deinitialization of any initialized dependecies

    return EXIT_SUCCESS;
}



// Lecture "Final Project Assignment Preparation"

// After the brake Zhivko explains about:
// 1) How to debug in a better way our application
// 2) How to achieve a better error reporting (even compilational error reporting)
// 3) How to benchmark our application

// build our application in debug buid -> cmake .. -DCMAKE_BUILD_TYPE=Debug
// ot tuk natatuk ima razlichni nachini s koito mozhe da "profile-vame nasheto prilozhenie"
// Nai-modernoto koeto mozhe da se polzva za 2021 godina sa taka narechenite SANITIZER, koito idvat ot kompilatorite
// Ima i za GCC ima i za CLANG, tezi za CLANG veroqtno sa po-dobri ot tezi na GCC, no tova ne znachi che tezi na GCC sa za
// izxvurlqne. Za da gi setup-nem obache shte otneme dosta vreme, zatova shte vi pokazha drug alternativen, mnogo silen variant
// kato edinstvenia mu drawback e che e baven, toi pravi prilozhenieto adski bavno. Tova e tool-a "valgrind"
// instalira se sus "sudo apt install valgrind". Sled tova runvame valgrind, kato toi ima razlichni tool-ove koito runva.
// Default-nia tool, tozi koito shte polzvame nie e "valgrind --tool=memory". Dori i da ne go napishem tova e default-nia.
// Sled tova sledvat nqkolko na broi opcii koito shte mu dadem "valgrind --leak-check=full" . Valgrind sledva vsichki
// patterni na alokacia na pamet v nasheto prilozhenie i go zapomnq. Taka che v posledstvie ako leak-ne pamet, mozhe da ni dade
// hint pone ot kude e trugnal tozi leak, a veche nie shte si go namerim po-konkretno. Druga vazhna opcia e 
// valgrind --leak-check=full --track-origins=yes . Track origins shte kazhe na valgrind da sledva vsqka edna inicializacia
// na promenliva i shte hvane ako imame neinicializirani promenlivi. Po-skoro shte gi hvane ako depend-vame na tqh.
// Primerno : imame "int a;" (bez da mu davame stoinost) i po-nadolu kazvame "if(a)". Tui-kato promenlivata e neinicializirana
// ponqkoga mozhe da e 0, ponqkoga mozhe da ne e 0 i tova shte e razlichno vseki put. Tova se naricha "conditional jump",koito
// depend-va na uninitialized variable i valgrind e mnogo dobur vuv xvashtaneto mu. Cqlosnata komanda e slednata:
// "valgrind --leak-check=full --track-origins=yes ./chess". Sled kato se startira prilozhenieto i porabotim s nego, pri izlizane
// ot prilozhenieto (primenro s ESC ili X butona) valgrind vadi report na greshkite koito sa se sluchili. Shtqhme da vidim report
// na geshka v momenta v koito tq se sluchi kato neinicializirana promenliva, a v kraq ni vadi generalen report na tova kakvo se e
// sluchilo. Generalnia report e "LEAK SUMMARY" : "definitely lost" -> sus sigurnost sme gi zagubili, "still reachable" ne znachi
// che sa leak-nati, a che kogato sme zatvorili nashata programa, ne sme gi osvobodili predi tova, no programata shte gi
// osvobodi taka ili inache, taka che vsichko e nared. 
// Vazno --> libX11 e grafichnata sreda na Linux !!!
// V Linux terminal-a, ako natisnem dva puti TAB burzo edin sled drug (i.e. za AUTO COMPLETE) mozhe da se listnat vuzmozni
// opcii. Primenro "valgrind -tool=" + TAB, TAB i se listvat tool-ovete. "cachegrind" tool-a e za sledene kude ima cache miss-ove
// v nasheto prilozhenie (i.e cache fit-ove), no tova e veche svrux optimizaciq do koqto shte stignem, no na po-kusen etap.
// Po-interesn e tool-a "callgrind". Toi meri vsichki funkcii koito sa izvikani v nasheto prilozhenie i gi broi. No osven broene,
// izmerva i kolko vreme sme prekarali tam. Po tozi nachin "callgrind" mozhe da napravi report kak se durzhi prilozhenieto.
// Koe otnema mnogo vreme v nego i koe otnema po-malko vreme. Sled kato napishem "valgrind --tool=callgrind ./chess" i kogato
// zatvorim prilozhenieto, vizhdame suzdaden file s imeto "callgrind.out.xxxxx". Tova e izgenerirania report. Vednaga runvame
// edin drug tool. Prosto v terminala pishem SAMO I EDINSTVENO slednia red: "kcachegrind callgrind.out.xxxxx &" 
// Otvarq se KCachegrind prozorec. V nego izbirame mezhdu Instruction Fetch ili Cycle Estimation. Mozhe bi Cycle Estimation
// e po-dobre da se izbere za tozi moment - tova shte kazhe kolko cycles e bilo neobxodimo na CPU-to da izpulni dadena operacia.
// Pogledni lekciata okolo 1:53:00 -> Zhivko razglezhda kak vurvi prilozhenieto s tozi "kcachegrind" tool.


// 2) Kak da si podobrim kompilacionnia error reporting
// Tova stava kato smenim kompilatora. V momenta polzvame default-nia Linux kompilator GCC -> C kompilatora 
// i g++ -> C++ kompilatora. Ima oshte edin mnogo populqreb variant: clang -> C kompilator i clang++ -> C++ kompilator
// Po default nqmame takuv na Ubuntu, no instalaciata mu e "sudo apt install clang"
// CMake e tool koito ni dava cross-platform-en nachin da setup-vame nashia proekt, da smenqme toolchain-a
// komandata "cmake .." run-va default-nia kompilator s default-nia generator. 
// "cmake -G" pokazva kakvi sa generatorite. Nie polzvame default-nia takuv za Linux -> Unix Makefiles
// eksplicitnata komanda e "cmake .. "Unix Makefiles""
// sledvashta opcia koqto mozhe da daden e "cmake .. -DCMAKE_CXX_COMPILER" i mu kazvame kude se namira nashia kompilator.
// ako iskam samo za C ezika ( ne e za C++ ) prosto mahame XX -> "cmake .. -DCMAKE_C_COMPILER"
// estestveno po default ne zname kude se namira kompilatora koito iskame. Zatova pishem v terminala "whereis g++" primerno.
// finalno pishem slednoto "cmake .. -DCMAKE_CXX_COMPILER=/usr/bin/clang++" primerno. Posle standartno pishem "make -j" za da
// kompilirame s novia izbran kompilator.
// Clang se oplakva, tui-kato se okazva che vuv faila helpers.cmake, vuv funkciata "target_compile_options()" nqkoi opcii sa
// GCC specific. Takiva mai sa "-Wlogical-op", "-Wduplicated-branches", "-Wduplicated-cond". Kato puk Clang si ima negovi si
// warnings koito mozhe da dobavim. Smenqme kompilatora na clang zashtoto celta ni e da poluchim po-dobur reporting ot 
// "statical analysis tools". Warning-ite sa tochno statical analysis tool-ove. Da gi hvanem po vreme na compile time.
// Clang e mnogo po-restriktiven kompilator. Toi shte se oplakva za vsichko. No taka nashia code shte bude pedantichen i shte
// ni spesti bugove. Ako smqtame che neshto e adski gadno i drazneshto, vinagi mozhe da izkluchim warning-a koito go trigger-va
// oshte poveche che v tozi chess proekt nie eksplicitno enforce-vame (v helpers.cmake faila) vseki warning da se tretira 
// kato kompilacionna greshka. Zhivko smqta che chovek trqbva da bude maksimalno pedantichen zashtoto v ezici kato C/C++
// lesno mozhes da ti grumnesh celia krak dori s edin red code. Razbira se drugoto kofti e che kogato checkout-vame chuzhd
// code, toi mozhe da ima warnings koito nie nqma kak da opravim i dazhe ne iskame da fixvame.
