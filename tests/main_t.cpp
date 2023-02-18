#include <gtest/gtest.h>
#include "ProtocolHandler/hdlc_handler.h"

TEST(HDLC_Handler_class, ByteCoverterTest_1)
{
    HDLC_Handler handler;
    std::list<uint8_t>bit_combination{ 0,1,1,1,1,1,1,0 };
    std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(handler.BitToByteConverter(bit_combination))).str();
    EXPECT_EQ(hex, "0x7e");
}

TEST(HDLC_Handler_class, ByteCoverterTest_2)
{
    HDLC_Handler handler;
    std::list<uint8_t>bit_combination{ 0,1,0,1,0,1,0,1 };
    std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(handler.BitToByteConverter(bit_combination))).str();
    EXPECT_EQ(hex, "0xaa");
}

TEST(HDLC_Handler_class, ByteCoverterTest_3)
{
    HDLC_Handler handler;
    std::list<uint8_t>bit_combination{ 1,0,0,0,0,0,0,1 };
    std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(handler.BitToByteConverter(bit_combination))).str();
    EXPECT_EQ(hex, "0x81");
}

TEST(HDLC_Handler_class, ByteCoverterTest_4)
{
    HDLC_Handler handler;
    std::list<uint8_t>bit_combination{ 1,0,1,1,0,0,1,1 };
    std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(handler.BitToByteConverter(bit_combination))).str();
    EXPECT_EQ(hex, "0xcd");
}

TEST(HDLC_Handler_class, fillBufferTest_1)
{
    HDLC_Handler handler;
    int correct_data_value = handler.fillBitBuffer("../data/HDLC_TEST_.BIT");
    EXPECT_EQ(correct_data_value, 0);
}

TEST(HDLC_Handler_class, fillBufferTest_2)
{
    HDLC_Handler handler;
    int wrong_data_value = handler.fillBitBuffer("../data/WRONG_DATA_.BIT");
    EXPECT_EQ(wrong_data_value, 1);
}

TEST(HDLC_Handler_class, fillBufferTest_3)
{
    HDLC_Handler handler;
    handler.fillBitBuffer("../data/HDLC_TEST_.BIT");
    int bit_buffer_size = handler.m_bit_buffer.size();
    int file_size_bytes = 10158062;
    EXPECT_EQ(bit_buffer_size, file_size_bytes * 8);
}

TEST(HDLC_Handler_class, incrementPackageIndexTest_1)
{
    HDLC_Handler handler;
    int value = 1;
    handler.incrementPackageIndex(value);
    EXPECT_EQ(value, 2);
}

TEST(HDLC_Handler_class, incrementPackageIndexTest_2)
{
    HDLC_Handler handler;
    int value = 2;
    handler.incrementPackageIndex(value);
    EXPECT_EQ(value, 3);
}

TEST(HDLC_Handler_class, getIndexOfDeletedBitTest_1)
{
    HDLC_Handler handler;
    int bit_package_index = 604;  
    int bit_buffer_index = 579;
    int indexOfDeleteBit = handler.getIndexOfDeletedBit(bit_package_index, bit_buffer_index); //604-579+8-0 
    EXPECT_EQ(indexOfDeleteBit, 33);
}

TEST(HDLC_Handler_class, getIndexOfDeletedBitTest_2)
{
    HDLC_Handler handler;
    int bit_package_index = 721;
    int bit_buffer_index = 579;
    int indexOfDeleteBit = handler.getIndexOfDeletedBit(bit_package_index, bit_buffer_index); //721-579+8-0 
    EXPECT_EQ(indexOfDeleteBit, 150);
}

TEST(HDLC_Handler_class, getIndexOfDeletedBitTest_3)
{
    HDLC_Handler handler;
    int bit_package_index = 762;
    int bit_buffer_index = 579;
    int indexOfDeleteBit = handler.getIndexOfDeletedBit(bit_package_index, bit_buffer_index); //762-579+8-0 
    EXPECT_EQ(indexOfDeleteBit, 191);
}

//the situation when the container is filled indexes are shifted:

TEST(HDLC_Handler_class, getIndexOfDeletedBitTest_4)
{
    HDLC_Handler handler;
    int bit_package_index = 604;
    int bit_buffer_index = 579;
    int indexOfDeleteBit = handler.getIndexOfDeletedBit(bit_package_index, bit_buffer_index); //604-579+8-0 
    indexOfDeleteBit -= 0;
    EXPECT_EQ(indexOfDeleteBit, 33);
}

TEST(HDLC_Handler_class, getIndexOfDeletedBitTest_5)
{
    HDLC_Handler handler;
    int bit_package_index = 721;
    int bit_buffer_index = 579;
    int indexOfDeleteBit = handler.getIndexOfDeletedBit(bit_package_index, bit_buffer_index); //721-579+8-1 
    indexOfDeleteBit -= 1;
    EXPECT_EQ(indexOfDeleteBit, 149);
}

TEST(HDLC_Handler_class, getIndexOfDeletedBitTest_6)
{
    HDLC_Handler handler;
    int bit_package_index = 762;
    int bit_buffer_index = 579;
    int indexOfDeleteBit = handler.getIndexOfDeletedBit(bit_package_index, bit_buffer_index); //762-579+8-2 
    indexOfDeleteBit -= 2;
    EXPECT_EQ(indexOfDeleteBit, 189);
}

TEST(HDLC_Handler_class, addBitToPackageTest) {
    HDLC_Handler handler;
    handler.fillBitBuffer("../data/HDLC_TEST_.BIT");
    handler.addBitToPackage(handler.m_bit_buffer, 1);
    EXPECT_EQ(handler.m_package.size(), 1);

    handler.addBitToPackage(handler.m_bit_buffer, 2);
    EXPECT_EQ(handler.m_package.size(), 2);

    handler.addBitToPackage(handler.m_bit_buffer, 3);
    EXPECT_EQ(handler.m_package.size(), 3);

    handler.addBitToPackage(handler.m_bit_buffer, 10);
    EXPECT_EQ(handler.m_package.size(), 4);
}

TEST(HDLC_Handler_class, addBitToByteBufferTest) {
    HDLC_Handler handler;
    uint8_t bit = 0;
    handler.addBitToByteBuffer(bit);
    EXPECT_EQ(handler.m_byte_buffer.size(), 1);
    handler.addBitToByteBuffer(bit);
    EXPECT_EQ(handler.m_byte_buffer.size(), 2);
    handler.addBitToByteBuffer(bit);
    EXPECT_EQ(handler.m_byte_buffer.size(), 3);
}

TEST(HDLC_Handler_class, MakeStepInSequenceOfBitBufferTest) {
    HDLC_Handler handler;
    handler.fillBitBuffer("../data/HDLC_TEST_.BIT");   //full bit sequence [1,1,1,1,0,0] (starting with the sixth bit HDLC_TEST_.BIT)
    std::list<int> bit_sequence = {0,0,0,0,0,1};
    int current_index = 6;
    handler.MakeStepInSequenceOfBitBuffer(bit_sequence, current_index);
    std::list<int> bit_sequence_result_1 = { 0,0,0,0,1,1 };
    ASSERT_EQ(bit_sequence, bit_sequence_result_1);

    handler.MakeStepInSequenceOfBitBuffer(bit_sequence, current_index);
    std::list<int> bit_sequence_result_2 = { 0,0,0,1,1,1 };
    ASSERT_EQ(bit_sequence, bit_sequence_result_2);

    handler.MakeStepInSequenceOfBitBuffer(bit_sequence, current_index);
    std::list<int> bit_sequence_result_3 = { 0,0,1,1,1,1 };
    ASSERT_EQ(bit_sequence, bit_sequence_result_3);

    handler.MakeStepInSequenceOfBitBuffer(bit_sequence, current_index);
    std::list<int> bit_sequence_result_4 = { 0,1,1,1,1,0 };
    ASSERT_EQ(bit_sequence, bit_sequence_result_4);

    handler.MakeStepInSequenceOfBitBuffer(bit_sequence, current_index);
    std::list<int> bit_sequence_result_5 = { 1,1,1,1,0,0 };
    ASSERT_EQ(bit_sequence, bit_sequence_result_5);
}

TEST(HDLC_Handler_class, isBitBufferLimitTest) {
    HDLC_Handler handler;
    handler.fillBitBuffer("../data/HDLC_TEST_.BIT");
    int current_package_index = 10158062 * 8 - 1;       //(total bytes * number of bites)
    bool result = handler.isBitBufferLimit(current_package_index, handler.m_bit_buffer);
    EXPECT_EQ(true, result);

    current_package_index = 10238062 * 8 - 1;           //(random bytes * number of bites)
    result = handler.isBitBufferLimit(current_package_index, handler.m_bit_buffer);
    EXPECT_EQ(false, result);

    current_package_index = 422323 * 8 - 1;             //(random bytes * number of bites)
    result = handler.isBitBufferLimit(current_package_index, handler.m_bit_buffer);
    EXPECT_EQ(false, result);
}

TEST(HDLC_Handler_class, checkSequenceforDuplicateTest) {
    HDLC_Handler handler;
    handler.fillBitBuffer("../data/HDLC_TEST_.BIT");
    int bit_package_index = 239274;
    int bit_buffer_index = 238986;
    int drop_element_index = 291;
    bool duplicate = handler.checkSequenceforDuplicate(bit_package_index, bit_buffer_index, drop_element_index);
    EXPECT_EQ(true, duplicate);
    int drop_element_index_for_duplicate = handler.m_numbers_of_delete_bit[0];
    EXPECT_EQ(297, drop_element_index_for_duplicate); 
}

TEST(HDLC_Handler_class, checkSequenceForFirstEntryBitFlagTest) {
    HDLC_Handler handler;
    handler.fillBitBuffer("../data/HDLC_TEST_.BIT");   
    int bit_package_index = 1983339;
    int bit_buffer_index = 1983341;
    int drop_element_index = 6;
    handler.m_package = { 1,1,1,1,1,0 };
    bool duplicate_state_one = handler.checkSequenceForFirstEntryBitFlag(bit_package_index, bit_buffer_index, drop_element_index);
    EXPECT_EQ(true, duplicate_state_one);
    int drop_element_index_for_duplicate = handler.m_numbers_of_delete_bit[0];
    EXPECT_EQ(6, drop_element_index);
    handler.m_package = { 0,0,0,0,0,0 };
    bool duplicate_state_two = handler.checkSequenceForFirstEntryBitFlag(bit_package_index, bit_buffer_index, drop_element_index);
    EXPECT_EQ(false, duplicate_state_two);
}

TEST(HDLC_Handler_class, shiftIndexOfBufferBitTest) {
    HDLC_Handler handler;
    handler.fillBitBuffer("../data/HDLC_TEST_.BIT");
    int start_bit_buffer_index = 0;
    handler.m_package = { 1,0,1,1,1,1,0,0,0,0,0,0,1,0,1,1,0,1,0,0,0,0,0,1,0,1,1,0 }; //random bit sequence with no meaning
    handler.m_numbers_of_delete_bit = { 2,5,6,7,8 };                                 //random numbers with no meaning
    handler.shiftIndexOfBufferBit(start_bit_buffer_index);
    EXPECT_EQ(33, start_bit_buffer_index);

    start_bit_buffer_index = 15;
    handler.m_package = { 1,0,1,1,1,1,0,0,0,0 }; 
    handler.m_numbers_of_delete_bit = { 2,4,6,8,10,12 };  
    handler.shiftIndexOfBufferBit(start_bit_buffer_index);
    EXPECT_EQ(31, start_bit_buffer_index);
}


TEST(HDLC_Handler_class, getFirstFlagBitTest) {
    HDLC_Handler handler;
    handler.fillBitBuffer("../data/HDLC_TEST_.BIT");
    std::vector<uint8_t>m_bit_frame_flag = { 0,1,1,1,1,1,1,0 };
    int index_of_bit = handler.getFirstFlagBit(m_bit_frame_flag, handler.m_bit_buffer);
    EXPECT_EQ(3, index_of_bit);
}