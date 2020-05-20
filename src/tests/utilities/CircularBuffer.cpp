#include <gtest/gtest.h>
#include <utilities/CircularBuffer.h>

TEST(CircularBuffer, TestBasicAttributes)
{
    CircularBuffer cb(1000);

    EXPECT_EQ(1000, cb.getCapacity());
    EXPECT_EQ(1000, cb.getRemaining());
    EXPECT_EQ(0, cb.getFillLevel());
    EXPECT_FALSE(cb.full());
    EXPECT_TRUE(cb.empty());
}

TEST(CircularBuffer, TestBasicInsert)
{
    CircularBuffer cb(100);

    EXPECT_TRUE(cb.empty());

    cb.write(reinterpret_cast<const uint8_t*>("Some Data!"), 10);

    EXPECT_EQ(10, cb.getFillLevel());
    EXPECT_FALSE(cb.full());
    EXPECT_FALSE(cb.empty());
}

TEST(CircularBuffer, TestBasicRetrieval)
{
    CircularBuffer cb(100);

    EXPECT_TRUE(cb.empty());

    cb.write(reinterpret_cast<const uint8_t*>("Some Data!"), 10);

    char buffer[11] = {0};
    size_t got = cb.read(reinterpret_cast<uint8_t*>(buffer), 10);
    ASSERT_EQ(10, got);
    EXPECT_STREQ("Some Data!", buffer);
    EXPECT_TRUE(cb.empty());
}

TEST(CircularBuffer, TestConsecutiveInserts)
{
    CircularBuffer cb(100);

    EXPECT_TRUE(cb.empty());

    cb.write(reinterpret_cast<const uint8_t*>("Some "), 5);
    cb.write(reinterpret_cast<const uint8_t*>("Data!"), 5);

    char buffer[11] = {0};
    size_t got = cb.read(reinterpret_cast<uint8_t*>(buffer), 10);
    ASSERT_EQ(10, got);
    EXPECT_STREQ("Some Data!", buffer);
    EXPECT_TRUE(cb.empty());
    EXPECT_FALSE(cb.full());
}

// This also tests a completely full buffer in various states
TEST(CircularBuffer, TestWraparoundRetrieval)
{
    CircularBuffer cb(10);

    EXPECT_TRUE(cb.empty());

    cb.write(reinterpret_cast<const uint8_t*>("Some Data!"), 10);
    EXPECT_TRUE(cb.full());

    char buffer[11] = {0};
    // read some of the buffer out
    size_t got = cb.read(reinterpret_cast<uint8_t*>(buffer), 5);
    EXPECT_EQ(5, got);
    EXPECT_EQ(5, cb.getFillLevel());

    // now reinsert to refill
    cb.write(reinterpret_cast<const uint8_t*>("Some "), 5);
    EXPECT_EQ(10, cb.getFillLevel());
    EXPECT_TRUE(cb.full());
    got = cb.read(reinterpret_cast<uint8_t*>(buffer), 10);
    EXPECT_EQ(10, got);
    EXPECT_STREQ("Data!Some ", buffer);
    EXPECT_TRUE(cb.empty());
}

TEST(CircularBuffer, TestReadEmptyBuffer)
{
    CircularBuffer cb(10);
    EXPECT_TRUE(cb.empty());

    char buffer[11] = {0};
    size_t got = cb.read(reinterpret_cast<uint8_t*>(buffer), 10);
    EXPECT_EQ(0, got);
}

TEST(CircularBuffer, TestReadingMoreThanContents)
{
    CircularBuffer cb(10);
    EXPECT_TRUE(cb.empty());

    cb.write(reinterpret_cast<const uint8_t*>("Data!"), 5);

    char buffer[11] = {0};
    size_t got = cb.read(reinterpret_cast<uint8_t*>(buffer), 10);
    EXPECT_EQ(5, got);

    EXPECT_STREQ("Data!", buffer);
}