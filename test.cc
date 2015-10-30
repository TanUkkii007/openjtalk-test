#include "gtest/gtest.h"
#include "open_jtalk_lib.h"

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
    
    TEST(OpenJtalkTest, Mecab_analysis2) {
        Mecab mecab;
        Mecab_initialize(&mecab);
        int argc = 3;
        std::string thisfile(__FILE__);
        std::string basedir = thisfile.substr(0, thisfile.find_last_of("/"));
        std::string dicdir = basedir + "/node_modules/node-openjtalk/build/Release";
        const char* argv[3] = {"mecab", "-d", dicdir.c_str()};
        
        mecab.mecab = mecab_new(argc, const_cast<char**>(argv));
        
        if (Mecab_analysis(&mecab, "Ｊａｖａｓｃｒｉｐｔとは、はたして本物か？")) {
            // Mecab#size
            EXPECT_EQ(mecab.size, 8);
            
            const char* answer[] = {"Ｊａｖａｓｃｒｉｐｔ", "と", "は", "、", "はたして", "本物", "か", "？"};
            
            // Mecab#feature[]
            for (int i = 0; i < mecab.size; ++i) {
                std::string featstr(mecab.feature[i]);
                printf("%s\n", featstr.c_str());
                //ASSERT_STREQ(answer[i], featstr.substr(0, featstr.find_first_of(",")).c_str());
            }
        }
    }
    
    TEST(OpenJtalkTest, mecab2njd) {
        NJD njd;
        NJD_initialize(&njd);
        int size = 7;
        const char* feature[] = {
            "すもも,名詞,一般,*,*,*,*,すもも,スモモ,スモモ,0/3,C2",
            "も,助詞,係助詞,*,*,*,*,も,モ,モ,0/1,名詞%F1/動詞%F2@0/形容詞%F2@0",
            "もも,名詞,一般,*,*,*,*,もも,モモ,モモ,0/2,C3",
            "も,助詞,係助詞,*,*,*,*,も,モ,モ,0/1,名詞%F1/動詞%F2@0/形容詞%F2@0",
            "もも,名詞,一般,*,*,*,*,もも,モモ,モモ,0/2,C3",
            "の,助詞,連体化,*,*,*,*,の,ノ,ノ,0/1,動詞%F2@0/形容詞%F1",
            "うち,名詞,非自立,副詞可能,*,*,*,うち,ウチ,ウチ,0/2,C3"
        };
        mecab2njd(&njd, const_cast<char**>(feature), size);
        NJDNode* node = njd.head;
        while (node != NULL) {
            printf("%s %s %s %s %s %s %s %s %d %d %s %d\n", node->string, node->pos, node->pos_group1, node->ctype, node->cform, node->orig, node->read ,node->pron, node->acc, node->mora_size, node->chain_rule, node->chain_flag);
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
    
}  // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}