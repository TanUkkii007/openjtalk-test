#include "gtest/gtest.h"
#include "mecab.h"
#include "njd.h"
#include "jpcommon.h"
#include "HTS_engine.h"
#include "HTS_hidden.h"

/* Sub headers */
#include "text2mecab.h"
#include "mecab2njd.h"
#include "njd_set_pronunciation.h"
#include "njd_set_digit.h"
#include "njd_set_accent_phrase.h"
#include "njd_set_accent_type.h"
#include "njd_set_unvoiced_vowel.h"
#include "njd_set_long_vowel.h"
#include "njd2jpcommon.h"

namespace {
    
    class OpenJtalkTest : public ::testing::Test {
    protected:
        
        OpenJtalkTest() {
            
        }
        
        virtual ~OpenJtalkTest() {
            
        }
        
        
        virtual void SetUp() {
            
        }
        
        virtual void TearDown() {
            
        }
        
    };
    
    
    TEST(OpenJtalkTest, Mecab_analysis) {
        Mecab mecab;
        Mecab_initialize(&mecab);
        int argc = 3;
        std::string thisfile(__FILE__);
        std::string basedir = thisfile.substr(0, thisfile.find_last_of("/"));
        std::string dicdir = basedir + "/node_modules/node-openjtalk/build/Release";
        const char* argv[3] = {"mecab", "-d", dicdir.c_str()};
        
        mecab.mecab = mecab_new(argc, const_cast<char**>(argv));
        
        if (Mecab_analysis(&mecab, "すもももももももものうち")) {
            // Mecab#size
            EXPECT_EQ(mecab.size, 7);
            
            const char* answer[] = {"すもも", "も", "もも", "も", "もも", "の", "うち"};
            
            // Mecab#feature[]
            for (int i = 0; i < mecab.size; ++i) {
                std::string featstr(mecab.feature[i]);
                printf("%s\n", featstr.c_str());
                ASSERT_STREQ(answer[i], featstr.substr(0, featstr.find_first_of(",")).c_str());
            }
        }
    }
    
    TEST(OpenJtalkTest, Mecab_analysis_accent) {
        Mecab mecab;
        Mecab_initialize(&mecab);
        int argc = 3;
        std::string thisfile(__FILE__);
        std::string basedir = thisfile.substr(0, thisfile.find_last_of("/"));
        std::string dicdir = basedir + "/node_modules/node-openjtalk/build/Release";
        const char* argv[3] = {"mecab", "-d", dicdir.c_str()};
        
        mecab.mecab = mecab_new(argc, const_cast<char**>(argv));
        
        if (Mecab_analysis(&mecab, "ありがとうございました")) {
            // Mecab#size
            EXPECT_EQ(mecab.size, 1);
            
            const char* answer[] = {"ありがとうございました"};
            
            // Mecab#feature[]
            for (int i = 0; i < mecab.size; ++i) {
                std::string featstr(mecab.feature[i]);
                printf("%s\n", featstr.c_str());
                ASSERT_STREQ(answer[i], featstr.substr(0, featstr.find_first_of(",")).c_str());
            }
        }
    }
    
    TEST(OpenJtalkTest, Mecab_analysis_desu) {
        Mecab mecab;
        Mecab_initialize(&mecab);
        int argc = 3;
        std::string thisfile(__FILE__);
        std::string basedir = thisfile.substr(0, thisfile.find_last_of("/"));
        std::string dicdir = basedir + "/node_modules/node-openjtalk/build/Release";
        const char* argv[3] = {"mecab", "-d", dicdir.c_str()};
        
        mecab.mecab = mecab_new(argc, const_cast<char**>(argv));
        
        if (Mecab_analysis(&mecab, "です？")) {
            // Mecab#size
            EXPECT_EQ(mecab.size, 2);
            
            const char* answer[] = {"です", "？"};
            
            // Mecab#feature[]
            for (int i = 0; i < mecab.size; ++i) {
                std::string featstr(mecab.feature[i]);
                printf("%s\n", featstr.c_str());
                ASSERT_STREQ(answer[i], featstr.substr(0, featstr.find_first_of(",")).c_str());
            }
        }
    }
    
    TEST(OpenJtalkTest, mecab2njd) {
        NJD njd;
        NJD_initialize(&njd);
        int size = 7 + 8;
        const char* feature[] = {
            "すもも,名詞,一般,*,*,*,*,すもも,スモモ,スモモ,0/3,C2",
            "も,助詞,係助詞,*,*,*,*,も,モ,モ,0/1,名詞%F1/動詞%F2@0/形容詞%F2@0",
            "もも,名詞,一般,*,*,*,*,もも,モモ,モモ,0/2,C3",
            "も,助詞,係助詞,*,*,*,*,も,モ,モ,0/1,名詞%F1/動詞%F2@0/形容詞%F2@0",
            "もも,名詞,一般,*,*,*,*,もも,モモ,モモ,0/2,C3",
            "の,助詞,連体化,*,*,*,*,の,ノ,ノ,0/1,動詞%F2@0/形容詞%F1",
            "うち,名詞,非自立,副詞可能,*,*,*,うち,ウチ,ウチ,0/2,C3",
            "Ｊａｖａｓｃｒｉｐｔ,名詞,固有名詞,組織,*,*,*,*",
            "と,助詞,格助詞,一般,*,*,*,と,ト,ト,0/1,形容詞%F1/動詞%F1",
            "は,助詞,係助詞,*,*,*,*,は,ハ,ワ,0/1,名詞%F1/動詞%F2@0/形容詞%F2@0",
            "、,記号,読点,*,*,*,*,、,、,、,*/*,*",
            "はたして,副詞,一般,*,*,*,*,はたして,ハタシテ,ハタシテ,2/4,*",
            "本物,名詞,一般,*,*,*,*,本物,ホンモノ,ホンモノ,0/4,C2",
            "か,助詞,副助詞／並立助詞／終助詞,*,*,*,*,か,カ,カ,0/1,名詞%F1/動詞%F2@0/形容詞%F2@0",
            "？,記号,一般,*,*,*,*,？,？,？,*/*,*"
        };
        mecab2njd(&njd, const_cast<char**>(feature), size);
        NJDNode* node = njd.head;
        while (node != NULL) {
            printf(" string: %s\n position: %s\n pos_group1: %s\n ctype: %s\n cform: %s\n original: %s\n read: %s\n pronunciation: %s\n accent: %d\n moraSize: %d\n chainRule: %s\n chainFlag: %d\n\n", node->string, node->pos, node->pos_group1, node->ctype, node->cform, node->orig, node->read ,node->pron, node->acc, node->mora_size, node->chain_rule, node->chain_flag);
            node = node->next;
        }
        
        // NJD#tail has the last node
        ASSERT_STREQ(njd.tail->string, "うち");
    }
    
    TEST(OpenJtalkTest, njd_set_pronunciation) {
        NJD njd;
        NJD_initialize(&njd);
        int size = 8;
        const char* feature[] = {
            "Ｊａｖａｓｃｒｉｐｔ,名詞,固有名詞,組織,*,*,*,*",
            "と,助詞,格助詞,一般,*,*,*,と,ト,ト,0/1,形容詞%F1/動詞%F1",
            "は,助詞,係助詞,*,*,*,*,は,ハ,ワ,0/1,名詞%F1/動詞%F2@0/形容詞%F2@0",
            "、,記号,読点,*,*,*,*,、,、,、,*/*,*",
            "はたして,副詞,一般,*,*,*,*,はたして,ハタシテ,ハタシテ,2/4,*",
            "本物,名詞,一般,*,*,*,*,本物,ホンモノ,ホンモノ,0/4,C2",
            "か,助詞,副助詞／並立助詞／終助詞,*,*,*,*,か,カ,カ,0/1,名詞%F1/動詞%F2@0/形容詞%F2@0",
            "？,記号,一般,*,*,*,*,？,？,？,*/*,*"
        };
        const char* answer[] = {"ジェーエイブイエイエスシーアールアイピーティー", "ト", "ワ", "、", "ハタシテ", "ホンモノ", "カ", "？"};
        mecab2njd(&njd, const_cast<char**>(feature), size);
        njd_set_pronunciation(&njd);
        
        NJDNode* node = njd.head;
        int i = 0;
        while (node != NULL) {
            printf("%s %s\n", node->string, node->pron);
            ASSERT_STREQ(node->pron, answer[i++]);
            node = node->next;
        }
        
        // "、" and "？" should not be removed.
        ASSERT_EQ(i, size);
    }
    
    TEST(OpenJtalkTest, njd_set_pronunciation_accent) {
        NJD njd;
        NJD_initialize(&njd);
        int size = 1;
        const char* feature[] = {
            "ありがとうございました,感動詞,*,*,*,*,*,ありがとう:ございました,アリガトウ:ゴザイマシタ,アリガトー:ゴザイマシ’タ,2/5:4/6,-1"
        };
        const char* answer[] = {"アリガトー", "ゴザイマシ’タ"};
        mecab2njd(&njd, const_cast<char**>(feature), size);
        njd_set_pronunciation(&njd);
        
        NJDNode* node = njd.head;
        int i = 0;
        while (node != NULL) {
            printf(" string: %s\n position: %s\n pos_group1: %s\n ctype: %s\n cform: %s\n original: %s\n read: %s\n pronunciation: %s\n accent: %d\n moraSize: %d\n chainRule: %s\n chainFlag: %d\n\n", node->string, node->pos, node->pos_group1, node->ctype, node->cform, node->orig, node->read ,node->pron, node->acc, node->mora_size, node->chain_rule, node->chain_flag);
            ASSERT_STREQ(node->pron, answer[i++]);
            node = node->next;
        }
        
        // "、" and "？" should not be removed.
        ASSERT_EQ(i, 2);
    }
    
    TEST(OpenJtalkTest, njd_set_pronunciation_desu) {
        NJD njd;
        NJD_initialize(&njd);
        int size = 2;
        const char* feature[] = {
            "です,助動詞,*,*,*,特殊・デス,基本形,です,デス,デス’,1/2,名詞%F2@1/動詞%F1/形容詞%F2@0",
            "？,記号,一般,*,*,*,*,？,？,？,*/*,*"
        };
        const char* answer[] = {"デス", "？"};
        mecab2njd(&njd, const_cast<char**>(feature), size);
        njd_set_pronunciation(&njd);
        
        NJDNode* node = njd.head;
        int i = 0;
        while (node != NULL) {
            printf(" string: %s\n position: %s\n pos_group1: %s\n ctype: %s\n cform: %s\n original: %s\n read: %s\n pronunciation: %s\n accent: %d\n moraSize: %d\n chainRule: %s\n chainFlag: %d\n\n", node->string, node->pos, node->pos_group1, node->ctype, node->cform, node->orig, node->read ,node->pron, node->acc, node->mora_size, node->chain_rule, node->chain_flag);
            ASSERT_STREQ(node->pron, answer[i++]);
            node = node->next;
        }
        
        // "、" and "？" should not be removed.
        ASSERT_EQ(i, size);
    }
    
    TEST(OpenJtalkTest, JPCommon_make_label) {
        NJD njd;
        NJD_initialize(&njd);
        int size = 1;
        const char* feature[] = {
            "ありがとうございました,感動詞,*,*,*,*,*,ありがとう:ございました,アリガトウ:ゴザイマシタ,アリガトー:ゴザイマシ’タ,2/5:4/6,-1"
        };
        const char* answer[] = {"アリガトー", "ゴザイマシ’タ"};
        mecab2njd(&njd, const_cast<char**>(feature), size);
        njd_set_pronunciation(&njd);
        JPCommon jpcommon;
        JPCommon_initialize(&jpcommon);
        njd2jpcommon(&jpcommon, &njd);
        JPCommon_make_label(&jpcommon);
        
        JPCommonNode *node = jpcommon.head;
        for (node = jpcommon.head; node != NULL; node = node->next) {
            printf("pron: %s, pos: %s, ctype: %s, cform: %s, chain_flag: %d\n", node->pron, node->pos, node->ctype, node->cform, node->chain_flag);
        }
        JPCommonLabel* label = jpcommon.label;
        JPCommonLabelPhoneme* phoneme;
        JPCommonLabelWord* word;
        JPCommonLabelAccentPhrase* accent;
        JPCommonLabelBreathGroup* breath;
        for (phoneme = label->phoneme_head; phoneme != NULL; phoneme = phoneme->next) {
            printf("phoneme: %s, mora: %s\n", phoneme->phoneme, phoneme->up->mora);
        }
        for (word = label->word_head; word != NULL; word = word->next) {
            printf("word: %s, pos: %s\n", word->pron, word->pos);
        }
        for (accent = label->accent_head; accent != NULL; accent = accent->next) {
            printf("accent: %d, emotion: %s\n", accent->accent, accent->emotion);
        }
        for (int i = 0; i < label->size; ++i) {
            printf("feature: %s\n", label->feature[i]);
        }
    }
    
    TEST(OpenJtalkTest, HTS_Engine_synthesize_from_strings) {
        std::string thisfile(__FILE__);
        std::string basedir = thisfile.substr(0, thisfile.find_last_of("/"));
        std::string voice = basedir + "/node_modules/node-openjtalk/voice/hts_voice_nitech_jp_atr503_m001-1.05 2/nitech_jp_atr503_m001.htsvoice";
        char* voices[] = {const_cast<char*>(voice.c_str())};
        HTS_Engine engine;
        HTS_Engine_initialize(&engine);
        HTS_Engine_load(&engine, voices, 1);
        
        NJD njd;
        NJD_initialize(&njd);
        int size = 1;
        const char* feature[] = {
            "ありがとうございました,感動詞,*,*,*,*,*,ありがとう:ございました,アリガトウ:ゴザイマシタ,アリガトー:ゴザイマシ’タ,2/5:4/6,-1"
        };

        mecab2njd(&njd, const_cast<char**>(feature), size);
        njd_set_pronunciation(&njd);
        JPCommon jpcommon;
        JPCommon_initialize(&jpcommon);
        njd2jpcommon(&jpcommon, &njd);
        JPCommon_make_label(&jpcommon);
        
        HTS_Engine_synthesize_from_strings(&engine, jpcommon.label->feature, jpcommon.label->size);
        
        HTS_LabelString* string;
        HTS_Question* question;
        for (string = engine.label.head; string != NULL; string = string->next) {
            printf("HTS_LabelString: %s, start: %f, end: %f\n", string->name, string->start, string->end);
        }
        
        for (question = engine.ms.duration->question; question != NULL; question = question->next) {
            printf("HTS_Question#head: %s\n", question->head->string);
        }
    }
    
    TEST(OpenJtalkTest, HTS_Vocoder_synthesize) {
        size_t m = 34;
        double lf0 = -10000000000.000000;
        size_t nlpf = 31;
        double alpha = 0.550000;
        double beta = 0.000000;
        double volume = 1.000000;
        double spectrum[] = {0.818595, 1.359464, 0.571320, 0.457894, 0.210681, 0.218052, 0.153583, 0.109085, 0.141901, 0.192637, 0.111708, 0.084790, 0.023146, 0.048018, 0.048468, 0.058492, 0.030779, 0.066592, 0.083832, 0.090839, 0.074053, 0.063111, 0.079848, 0.085349, 0.057884, 0.048058, 0.038772, 0.053205, 0.063751, 0.060228, 0.034588, 0.043284, 0.040019, 0.037399};
        double lpf[] = {-0.005523, -0.009832, -0.008729, -0.000428, 0.011955, 0.020770, 0.017779, 0.000000, -0.026049, -0.045020, -0.039150, 0.002198, 0.074922, 0.159196, 0.226591, 0.252320, 0.226591, 0.159196, 0.074922, 0.002198, -0.039150, -0.045020, -0.026049, 0.000000, 0.017779, 0.020770, 0.011955, -0.000428, -0.008729, -0.009832, -0.005523};
        
        size_t stage = 0;
        HTS_Boolean use_log_gain = FALSE;
        size_t sampling_rate = 48000;
        size_t fperiod = 240;
        HTS_Vocoder v;
        HTS_Vocoder_initialize(&v, m, stage, use_log_gain, sampling_rate, fperiod);
        double* rawdata = (double*) malloc(m * sizeof(double));
        HTS_Audio* audio = NULL;
        HTS_Vocoder_synthesize(&v, m, lf0, spectrum, nlpf, lpf, alpha, beta, volume, rawdata, audio);
        printf("rawdata = [");
        for (int i = 0; i < m; i++) {
            printf("%f, ", rawdata[i]);
        }
        printf("]\n");
        //free(rawdata);
    }

    
//    TEST(OpenJtalkTest, HTS_Engine_load) {
//        std::string thisfile(__FILE__);
//        std::string basedir = thisfile.substr(0, thisfile.find_last_of("/"));
//        std::string voice = basedir + "/node_modules/node-openjtalk/voice/hts_voice_nitech_jp_atr503_m001-1.05 2/nitech_jp_atr503_m001.htsvoice";
//        char* voices[] = {const_cast<char*>(voice.c_str())};
//        HTS_Engine engine;
//        HTS_Engine_initialize(&engine);
//        HTS_Engine_load(&engine, voices, 1);
//        ASSERT_EQ(engine.condition.sampling_frequency, 48000);
//    }
    
}  // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}