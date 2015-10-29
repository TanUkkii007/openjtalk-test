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
    
}  // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}