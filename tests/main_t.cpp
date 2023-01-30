#include <gtest/gtest.h>
#include "ProtocolHandler/hdlc_handler.h"

TEST(ByteCoverterTest_1, List)
{
    HDLC_Handler handler;
    std::list<size_t>bit_combination{ 0,1,1,1,1,1,1,0 };
    handler.byteConverterLst(bit_combination);
    std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(handler.byteConverterLst(bit_combination))).str();
    EXPECT_EQ(hex, "0x7e");
}

TEST(ByteCoverterTest_2, List)
{
    HDLC_Handler handler;
    std::list<size_t>bit_combination{ 0,1,0,1,0,1,0,1 };
    handler.byteConverterLst(bit_combination);
    std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(handler.byteConverterLst(bit_combination))).str();
    EXPECT_EQ(hex, "0xaa");
}

TEST(ByteCoverterTest_3, List)
{
    HDLC_Handler handler;
    std::list<size_t>bit_combination{ 1,0,0,0,0,0,0,1 };
    handler.byteConverterLst(bit_combination);
    std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(handler.byteConverterLst(bit_combination))).str();
    EXPECT_EQ(hex, "0x81");
}

TEST(ByteCoverterTest_4, List)
{
    HDLC_Handler handler;
    std::list<size_t>bit_combination{ 1,0,1,1,0,0,1,1 };
    handler.byteConverterLst(bit_combination);
    std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(handler.byteConverterLst(bit_combination))).str();
    EXPECT_EQ(hex, "0xcd");
}


TEST(ByteCoverterTest_1, Vector)
{
    HDLC_Handler handler;
    std::vector<size_t>bit_combination{ 0,1,1,1,1,1,1,0 };
    handler.byteConverterVec(bit_combination);
    std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(handler.byteConverterVec(bit_combination))).str();
    EXPECT_EQ(hex, "0x7e");
}

TEST(ByteCoverterTest_2, Vector)
{
    HDLC_Handler handler;
    std::vector<size_t>bit_combination{ 0,1,0,1,0,1,0,1 };
    handler.byteConverterVec(bit_combination);
    std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(handler.byteConverterVec(bit_combination))).str();
    EXPECT_EQ(hex, "0xaa");
}

TEST(ByteCoverterTest_3, Vector)
{
    HDLC_Handler handler;
    std::vector<size_t>bit_combination{ 1,0,0,0,0,0,0,1 };
    handler.byteConverterVec(bit_combination);
    std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(handler.byteConverterVec(bit_combination))).str();
    EXPECT_EQ(hex, "0x81");
}

TEST(ByteCoverterTest_4, Vector)
{
    HDLC_Handler handler;
    std::vector<size_t>bit_combination{ 1,0,1,1,0,0,1,1 };
    handler.byteConverterVec(bit_combination);
    std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(handler.byteConverterVec(bit_combination))).str();
    EXPECT_EQ(hex, "0xcd");
}