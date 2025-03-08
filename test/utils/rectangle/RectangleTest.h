#ifndef TEST_UTILS_RECTANGLE_RECTANGLETEST_H_
#define TEST_UTILS_RECTANGLE_RECTANGLETEST_H_

#include "gtest/gtest.h"

// v momenta v koito linknem kum edna biblioteka, tq si nosi sobstvenite "includes" sus sebe si...
// v sluchaq utils bibliotekata v neinia CMakeLists.txt kazva "target_include_directories()" i si include-va svoq include path
#include "utils/drawing/Rectangle.h"

class RectangleTest : public testing::Test {
// testing::Test pravi avtomatichno nashia class da bude testov class
// taka macro-to RUN_ALL_TESTS() shte razpoznae nashia class kato testov class i shte run-ne vsichki negovi testove
public:
    // nashia Ctor shte vikne Ctor-a na bazovia class Test
    // Sled kato se suzdade nashia obekt, ni se garantira che purvata funkcia koqto shte se izvika e funkciata "SetUp()"
    // I respektivno, poslednata funkcia koqto se izvikva, kogato se unishtozhi nashia Test, shte bude "TearDown()"
    // Realno Tezi dve funkcii sa "init()" i "deinit()" funkciite koito nie pisahme do momenta. Prosto testa gi vika...
    // nagotovo za nas
    RectangleTest();

    // Sets up the test fixture
    
    void SetUp() override {
        // trqbva da se override-nat
        // mozhe da si napishem nqkakuv custom code - ako imame nuzhda ot takuv
        // for instance can call the "init()" function of any object -> i.e. _engine->init()
    }

    // Tears Down the test fixture
    void TearDown() override {
        // trqbva da se override-nat
        // mozhe da si napishem nqkakuv custom code - ako imame nuzhda ot takuv
        // for instance can call the "deinit()" function of any object -> i.e. _engine->deinit()
    }

protected:
    Rectangle _rect;
    // Engine _engine; // added only to indicate that its "init()" and "deinit()" methods can be called in the functions above
};

// ima nqkolko marco-ta na Google Framework
// 1) TEST()
// 2) TEST_F() -> izpolzva se ako iskame da polzvame nqkakvi obekti v nashia class (kato _rect po-gore), ...
// ... zashtoto po princip ne e zadulzhitelno - mozhe da imame mnogo prosti testove


TEST_F(RectangleTest, isRectangleZero){ // RectangleTest se polzva tuk / realizirano e kato nasledqvane

    // izpolvame macro-ta za da pravim sravnenia i mozhe bi drugi neshta
    // EXPECT_EQ(first,second) -> vzima 2 neshta i ochakva da sa edni i sushti
    // ASSERT_TRUE() e sushtoto kato EXPECT_TRUE(), no  testa bi grumnal vednaga ako nqmame true...
    // ... dokato EXPECT_TRUE() shte otchete che e greshka, no shte produlzhi nadolu ako mislim...
    // ... che mozhe da izvlechem oshte informacia ot tozi test
    // EXPECT_FALSE() sushto mozhe
    EXPECT_TRUE(_rect==Rectangle::ZERO);

    EXPECT_TRUE(Rectangle::ZERO!=Rectangle::UNDEFINED);
}

TEST_F(RectangleTest, isPointInside){
    const Rectangle rect(0,0,1,1);
    const Point point1(0,0);
    const Point point2(1,1);

    EXPECT_TRUE(rect.isPointInside(point1));
    EXPECT_FALSE(rect.isPointInside(point2));
}

// NOTE: The good thing is that G-Test runs parallel if we have different targets
// In our case here we have one target, and all the tests in the target are executed one after the other - not parallel
// No ako primerno imaxme druga papka "DrawParams" i tq vutre v sebe si imashe drugo binnary...
// ... to dvete neshta (DrawParams i RectangleTest)  shtqha da vurvqt parallelno, koeto e super gotino

// IMPORTANT: Kak se izpolzva tova neshto v edna profesionalna sreda
// Kogato pishete Software, pishete testove za nego, sled tova se pishe programa koqto se naricha CICD...
// ... toest kachvame nqkude v Cloud-a instrukcii koito purvo build-vat nashia kod i proverqvat - toi build-va li se...
// ... super ako se build-va znachi generira nqkakvi failove/raboti, sled tova run-va testove, ako testovete minat...
// ... super minava na sledvashtata stupka, koqto e nqkakuv deployment (toest kachva go nqkude) i t.n. 
// No ako nqkoq ot stupkite fail-ne se spira do tam i nie poluchavame report-a nagotovo

// Zhivko: "S tova mislq da priklucha otgovora na vtoria vupros na Martin otnosno testing-a.
// Tova beshe ednata biblioteka GoogleTest, a drugata e GoogleMock (GMock), S GMock mozhe da kazvame neshta ot sorta na...
// puskame "engine-a" i na nashia mock-nat obekt (i.e. na nashia testov obekt) da mu kazhem "engine.expect(draw).AtLeastOnce" ili...
// "engine.expect(draw).AtLeast(5)" -> toest ochakvai che nqkakva funkcia "draw()" shte bude izvikana pone vednuzh ili 5 puti...
// ... po tozi nachin mozhe da pravim po-advanced testove. Drugo koeto mozhe da pravim e da assert-vame che prilozhenieto...
// ni shte umre - da ochakvame che shte poluchim exception. Tova se naricha "death test". Toest ochakvame che pri nqkakuv...
// scenarii prilozhenieto ni shte grumne. Primerno imame masiv ot 50 elementa, podavame mu 51, check-vame za exception...
// nqkude za tova neshto - primerno ako polzvame "at" operatora na std::vector, to toi shte throw-ne exception i nasheto...
// prilozhenie / test shte hvane tozi exception i shte umre, no nie shte ochakvame to da umre, koeto shte e zelena svetlina...
// za testa. "

// Zhivko: "Estestveno choveka koito pishe testovete, mozhe da sgreshi pri samoto im pisane...
// Testovete ne sa panaceia. Te sa hubavo neshto, no ako chovek napishe 10,000 testa te iziskvat poddruzhka...
// V momenta v koito promenim neshto, trqbva da promenim vsichki testove, koito sa svurzani s tova neshto...
// Taka che bottom line-a e balance. Hubavo e da imame testove, no ne i da se otvlichame napulno v tazi posoka...
// Kato grafichni prilozhenie se testvat po-trudno. Zashtoto tezi testove, primerno server-a na koito vurvqt nqma display...
// taka che ako shte gi testvame, to trqbva da pomislim za nqkakva alternativa pri koqto nasheto prilozhenie da vurvi...
// bez grafika, ako pravim functional testove. Primerno s nqkakuv konfiguracionen flag da mu setvame dali naistina da...
// suzdava prozorec ili da ne suzdava takuv, dali naistina da risuvame ili da ne risuvame i t.n."



#endif