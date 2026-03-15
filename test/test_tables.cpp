#include <gtest/gtest.h>
#include "TArrayTable.hpp"
#include "TListTable.hpp"
#include "TOrderedArrayTable.hpp"
#include "TAVLTreeTable.hpp"
#include "TPolinom.hpp"

TEST(TArrayTableTest, DefaultConstructor)
{
  TArrayTable<TPolinom> table;
  EXPECT_EQ(table.GetSize(), 0);
  EXPECT_TRUE(table.IsEmpty());
}

TEST(TArrayTableTest, InsertAndFind)
{
  TArrayTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);
  
  table.Insert("test", poly);
  EXPECT_EQ(table.GetSize(), 1);
  EXPECT_FALSE(table.IsEmpty());
  
  TPolinom* found = table.Find("test");
  ASSERT_NE(found, nullptr);
  EXPECT_TRUE(*found == poly);
}

TEST(TArrayTableTest, Contains)
{
  TArrayTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);
  
  table.Insert("test", poly);
  EXPECT_TRUE(table.Contains("test"));
  EXPECT_FALSE(table.Contains("nonexistent"));
}

TEST(TArrayTableTest, Remove)
{
  TArrayTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);
  
  table.Insert("test", poly);
  EXPECT_TRUE(table.Remove("test"));
  EXPECT_EQ(table.GetSize(), 0);
  EXPECT_FALSE(table.Contains("test"));
  EXPECT_FALSE(table.Remove("nonexistent"));
}

TEST(TArrayTableTest, UpdateExistingKey)
{
  TArrayTable<TPolinom> table;
  TMonom m1(2.0, {1.0, 0.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0, 0.0});
  TPolinom poly1(m1);
  TPolinom poly2(m2);
  
  table.Insert("test", poly1);
  table.Insert("test", poly2);
  
  EXPECT_EQ(table.GetSize(), 1);
  TPolinom* found = table.Find("test");
  EXPECT_TRUE(*found == poly2);
}

TEST(TArrayTableTest, GetKeysAndGetAll)
{
  TArrayTable<TPolinom> table;
  TMonom m1(2.0, {1.0, 0.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0, 0.0});
  TPolinom poly1(m1);
  TPolinom poly2(m2);
  
  table.Insert("poly1", poly1);
  table.Insert("poly2", poly2);
  
  auto keys = table.GetKeys();
  EXPECT_EQ(keys.size(), 2);
  
  auto all = table.GetAll();
  EXPECT_EQ(all.size(), 2);
}

TEST(TArrayTableTest, Clear)
{
  TArrayTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);
  
  table.Insert("test", poly);
  table.Clear();
  EXPECT_EQ(table.GetSize(), 0);
  EXPECT_TRUE(table.IsEmpty());
}

TEST(TListTableTest, DefaultConstructor)
{
  TListTable<TPolinom> table;
  EXPECT_EQ(table.GetSize(), 0);
  EXPECT_TRUE(table.IsEmpty());
}

TEST(TListTableTest, InsertAndFind)
{
  TListTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);
  
  table.Insert("test", poly);
  EXPECT_EQ(table.GetSize(), 1);
  EXPECT_FALSE(table.IsEmpty());
  
  TPolinom* found = table.Find("test");
  ASSERT_NE(found, nullptr);
  EXPECT_TRUE(*found == poly);
}

TEST(TListTableTest, Contains)
{
  TListTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);
  
  table.Insert("test", poly);
  EXPECT_TRUE(table.Contains("test"));
  EXPECT_FALSE(table.Contains("nonexistent"));
}

TEST(TListTableTest, Remove)
{
  TListTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);
  
  table.Insert("test", poly);
  EXPECT_TRUE(table.Remove("test"));
  EXPECT_EQ(table.GetSize(), 0);
  EXPECT_FALSE(table.Contains("test"));
  EXPECT_FALSE(table.Remove("nonexistent"));
}

TEST(TListTableTest, UpdateExistingKey)
{
  TListTable<TPolinom> table;
  TMonom m1(2.0, {1.0, 0.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0, 0.0});
  TPolinom poly1(m1);
  TPolinom poly2(m2);
  
  table.Insert("test", poly1);
  table.Insert("test", poly2);
  
  EXPECT_EQ(table.GetSize(), 1);
  TPolinom* found = table.Find("test");
  EXPECT_TRUE(*found == poly2);
}

TEST(TOrderedArrayTableTest, DefaultConstructor)
{
  TOrderedArrayTable<TPolinom> table;
  EXPECT_EQ(table.GetSize(), 0);
  EXPECT_TRUE(table.IsEmpty());
}

TEST(TOrderedArrayTableTest, InsertAndFind)
{
  TOrderedArrayTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);
  
  table.Insert("test", poly);
  EXPECT_EQ(table.GetSize(), 1);
  EXPECT_FALSE(table.IsEmpty());
  
  TPolinom* found = table.Find("test");
  ASSERT_NE(found, nullptr);
  EXPECT_TRUE(*found == poly);
}

TEST(TOrderedArrayTableTest, OrderedInsertion)
{
  TOrderedArrayTable<TPolinom> table;
  TMonom m1(2.0, {1.0, 0.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0, 0.0});
  TMonom m3(1.0, {0.0, 0.0, 1.0});
  TPolinom poly1(m1), poly2(m2), poly3(m3);
  
  table.Insert("poly3", poly3);
  table.Insert("poly1", poly1);
  table.Insert("poly2", poly2);
  
  auto keys = table.GetKeys();
  EXPECT_EQ(keys[0], "poly1");
  EXPECT_EQ(keys[1], "poly2");
  EXPECT_EQ(keys[2], "poly3");
}

TEST(TOrderedArrayTableTest, Contains)
{
  TOrderedArrayTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);
  
  table.Insert("test", poly);
  EXPECT_TRUE(table.Contains("test"));
  EXPECT_FALSE(table.Contains("nonexistent"));
}

TEST(TOrderedArrayTableTest, Remove)
{
  TOrderedArrayTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);
  
  table.Insert("test", poly);
  EXPECT_TRUE(table.Remove("test"));
  EXPECT_EQ(table.GetSize(), 0);
  EXPECT_FALSE(table.Contains("test"));
  EXPECT_FALSE(table.Remove("nonexistent"));
}

TEST(TAVLTreeTableTest, DefaultConstructor)
{
  TAVLTreeTable<TPolinom> table;
  EXPECT_EQ(table.GetSize(), 0);
  EXPECT_TRUE(table.IsEmpty());
}

TEST(TAVLTreeTableTest, InsertAndFind)
{
  TAVLTreeTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);
  
  table.Insert("test", poly);
  EXPECT_EQ(table.GetSize(), 1);
  EXPECT_FALSE(table.IsEmpty());
  
  TPolinom* found = table.Find("test");
  ASSERT_NE(found, nullptr);
  EXPECT_TRUE(*found == poly);
}

TEST(TAVLTreeTableTest, OrderedTraversal)
{
  TAVLTreeTable<TPolinom> table;
  TMonom m1(2.0, {1.0, 0.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0, 0.0});
  TMonom m3(1.0, {0.0, 0.0, 1.0});
  TPolinom poly1(m1), poly2(m2), poly3(m3);
  
  table.Insert("poly3", poly3);
  table.Insert("poly1", poly1);
  table.Insert("poly2", poly2);
  
  auto keys = table.GetKeys();
  EXPECT_EQ(keys[0], "poly1");
  EXPECT_EQ(keys[1], "poly2");
  EXPECT_EQ(keys[2], "poly3");
}

TEST(TAVLTreeTableTest, Contains)
{
  TAVLTreeTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);
  
  table.Insert("test", poly);
  EXPECT_TRUE(table.Contains("test"));
  EXPECT_FALSE(table.Contains("nonexistent"));
}

TEST(TAVLTreeTableTest, Remove)
{
  TAVLTreeTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);
  
  table.Insert("test", poly);
  EXPECT_TRUE(table.Remove("test"));
  EXPECT_EQ(table.GetSize(), 0);
  EXPECT_FALSE(table.Contains("test"));
  EXPECT_FALSE(table.Remove("nonexistent"));
}

TEST(TAVLTreeTableTest, MultipleInsertionsAndRemovals)
{
  TAVLTreeTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);
  
  for (int i = 0; i < 10; ++i) {
    table.Insert("key" + std::to_string(i), poly);
  }
  
  EXPECT_EQ(table.GetSize(), 10);
  
  for (int i = 0; i < 5; ++i) {
    EXPECT_TRUE(table.Remove("key" + std::to_string(i)));
  }
  
  EXPECT_EQ(table.GetSize(), 5);
  
  for (int i = 5; i < 10; ++i) {
    EXPECT_TRUE(table.Contains("key" + std::to_string(i)));
  }
}

TEST(AllTablesTest, CopyConstructor)
{
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);
  
  TArrayTable<TPolinom> arrayTable1;
  arrayTable1.Insert("test", poly);
  TArrayTable<TPolinom> arrayTable2(arrayTable1);
  EXPECT_EQ(arrayTable2.GetSize(), 1);
  EXPECT_TRUE(arrayTable2.Contains("test"));
  
  TListTable<TPolinom> listTable1;
  listTable1.Insert("test", poly);
  TListTable<TPolinom> listTable2(listTable1);
  EXPECT_EQ(listTable2.GetSize(), 1);
  EXPECT_TRUE(listTable2.Contains("test"));
}