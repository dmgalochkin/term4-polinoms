#include <gtest/gtest.h>
#include "TArrayTable.hpp"
#include "TListTable.hpp"
#include "TOrderedArrayTable.hpp"
#include "TAVLTreeTable.hpp"
#include "THashTable.hpp"
#include "TRedBlackTree.hpp"
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

  TPolinom *found = table.Find("test");
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
  TPolinom *found = table.Find("test");
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

  TPolinom *found = table.Find("test");
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
  TPolinom *found = table.Find("test");
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

  TPolinom *found = table.Find("test");
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

  TPolinom *found = table.Find("test");
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

  for (int i = 0; i < 10; ++i)
  {
    table.Insert("key" + std::to_string(i), poly);
  }

  EXPECT_EQ(table.GetSize(), 10);

  for (int i = 0; i < 5; ++i)
  {
    EXPECT_TRUE(table.Remove("key" + std::to_string(i)));
  }

  EXPECT_EQ(table.GetSize(), 5);

  for (int i = 5; i < 10; ++i)
  {
    EXPECT_TRUE(table.Contains("key" + std::to_string(i)));
  }
}

TEST(THashTableTest, DefaultConstructor)
{
  THashTable<TPolinom> table;
  EXPECT_EQ(table.GetSize(), 0);
  EXPECT_TRUE(table.IsEmpty());
}

TEST(THashTableTest, InsertAndFind)
{
  THashTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);

  table.Insert("test", poly);
  EXPECT_EQ(table.GetSize(), 1);
  EXPECT_FALSE(table.IsEmpty());

  TPolinom *found = table.Find("test");
  ASSERT_NE(found, nullptr);
  EXPECT_TRUE(*found == poly);
}

TEST(THashTableTest, Contains)
{
  THashTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);

  table.Insert("test", poly);
  EXPECT_TRUE(table.Contains("test"));
  EXPECT_FALSE(table.Contains("nonexistent"));
}

TEST(THashTableTest, Remove)
{
  THashTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);

  table.Insert("test", poly);
  EXPECT_TRUE(table.Remove("test"));
  EXPECT_EQ(table.GetSize(), 0);
  EXPECT_FALSE(table.Contains("test"));
  EXPECT_FALSE(table.Remove("nonexistent"));
}

TEST(THashTableTest, UpdateExistingKey)
{
  THashTable<TPolinom> table;
  TMonom m1(2.0, {1.0, 0.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0, 0.0});
  TPolinom poly1(m1);
  TPolinom poly2(m2);

  table.Insert("test", poly1);
  table.Insert("test", poly2);

  EXPECT_EQ(table.GetSize(), 1);
  TPolinom *found = table.Find("test");
  EXPECT_TRUE(*found == poly2);
}

TEST(THashTableTest, GetKeysAndGetAll)
{
  THashTable<TPolinom> table;
  TMonom m1(2.0, {1.0, 0.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0, 0.0});
  TPolinom poly1(m1);
  TPolinom poly2(m2);

  table.Insert("poly1", poly1);
  table.Insert("poly2", poly2);

  auto keys = table.GetKeys();
  EXPECT_EQ(keys.size(), 2);
  EXPECT_TRUE(std::find(keys.begin(), keys.end(), "poly1") != keys.end());
  EXPECT_TRUE(std::find(keys.begin(), keys.end(), "poly2") != keys.end());

  auto all = table.GetAll();
  EXPECT_EQ(all.size(), 2);
  EXPECT_TRUE(std::find_if(all.begin(), all.end(),
                           [](const auto &p)
                           { return p.first == "poly1"; }) != all.end());
  EXPECT_TRUE(std::find_if(all.begin(), all.end(),
                           [](const auto &p)
                           { return p.first == "poly2"; }) != all.end());
}

TEST(THashTableTest, Clear)
{
  THashTable<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);

  table.Insert("test", poly);
  table.Clear();
  EXPECT_EQ(table.GetSize(), 0);
  EXPECT_TRUE(table.IsEmpty());
}

TEST(TRedBlackTreeTest, DefaultConstructor)
{
  TRedBlackTree<TPolinom> table;
  EXPECT_EQ(table.GetSize(), 0);
  EXPECT_TRUE(table.IsEmpty());
}

TEST(TRedBlackTreeTest, InsertAndFind)
{
  TRedBlackTree<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);

  table.Insert("test", poly);
  EXPECT_EQ(table.GetSize(), 1);
  EXPECT_FALSE(table.IsEmpty());

  TPolinom *found = table.Find("test");
  ASSERT_NE(found, nullptr);
  EXPECT_TRUE(*found == poly);
}

TEST(TRedBlackTreeTest, Contains)
{
  TRedBlackTree<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);

  table.Insert("test", poly);
  EXPECT_TRUE(table.Contains("test"));
  EXPECT_FALSE(table.Contains("nonexistent"));
}

TEST(TRedBlackTreeTest, Remove)
{
  TRedBlackTree<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);

  table.Insert("test", poly);
  EXPECT_TRUE(table.Remove("test"));
  EXPECT_EQ(table.GetSize(), 0);
  EXPECT_FALSE(table.Contains("test"));
  EXPECT_FALSE(table.Remove("nonexistent"));
}

TEST(TRedBlackTreeTest, UpdateExistingKey)
{
  TRedBlackTree<TPolinom> table;
  TMonom m1(2.0, {1.0, 0.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0, 0.0});
  TPolinom poly1(m1);
  TPolinom poly2(m2);

  table.Insert("test", poly1);
  table.Insert("test", poly2);

  EXPECT_EQ(table.GetSize(), 1);
  TPolinom *found = table.Find("test");
  EXPECT_TRUE(*found == poly2);
}

TEST(TRedBlackTreeTest, OrderedTraversal)
{
  TRedBlackTree<TPolinom> table;
  TMonom m1(2.0, {1.0, 0.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0, 0.0});
  TMonom m3(1.0, {0.0, 0.0, 1.0});
  TPolinom poly1(m1), poly2(m2), poly3(m3);

  table.Insert("poly3", poly3);
  table.Insert("poly1", poly1);
  table.Insert("poly2", poly2);

  auto keys = table.GetKeys();
  EXPECT_EQ(keys.size(), 3);
  EXPECT_EQ(keys[0], "poly1");
  EXPECT_EQ(keys[1], "poly2");
  EXPECT_EQ(keys[2], "poly3");

  auto all = table.GetAll();
  EXPECT_EQ(all.size(), 3);
  EXPECT_EQ(all[0].first, "poly1");
  EXPECT_EQ(all[1].first, "poly2");
  EXPECT_EQ(all[2].first, "poly3");
}

TEST(TRedBlackTreeTest, MultipleInsertionsAndRemovals)
{
  TRedBlackTree<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);

  for (int i = 0; i < 10; ++i)
  {
    table.Insert("key" + std::to_string(i), poly);
  }

  EXPECT_EQ(table.GetSize(), 10);

  for (int i = 0; i < 5; ++i)
  {
    EXPECT_TRUE(table.Remove("key" + std::to_string(i)));
  }

  EXPECT_EQ(table.GetSize(), 5);

  for (int i = 5; i < 10; ++i)
  {
    EXPECT_TRUE(table.Contains("key" + std::to_string(i)));
  }
}

TEST(TRedBlackTreeTest, Clear)
{
  TRedBlackTree<TPolinom> table;
  TMonom m(2.0, {1.0, 0.0, 0.0});
  TPolinom poly(m);

  table.Insert("test", poly);
  table.Clear();
  EXPECT_EQ(table.GetSize(), 0);
  EXPECT_TRUE(table.IsEmpty());
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

  TOrderedArrayTable<TPolinom> orderedTable1;
  orderedTable1.Insert("test", poly);
  TOrderedArrayTable<TPolinom> orderedTable2(orderedTable1);
  EXPECT_EQ(orderedTable2.GetSize(), 1);
  EXPECT_TRUE(orderedTable2.Contains("test"));

  TAVLTreeTable<TPolinom> avlTable1;
  avlTable1.Insert("test", poly);
  TAVLTreeTable<TPolinom> avlTable2(avlTable1);
  EXPECT_EQ(avlTable2.GetSize(), 1);
  EXPECT_TRUE(avlTable2.Contains("test"));

  THashTable<TPolinom> hashTable1;
  hashTable1.Insert("test", poly);
  THashTable<TPolinom> hashTable2(hashTable1);
  EXPECT_EQ(hashTable2.GetSize(), 1);
  EXPECT_TRUE(hashTable2.Contains("test"));

  TRedBlackTree<TPolinom> rbTable1;
  rbTable1.Insert("test", poly);
  TRedBlackTree<TPolinom> rbTable2(rbTable1);
  EXPECT_EQ(rbTable2.GetSize(), 1);
  EXPECT_TRUE(rbTable2.Contains("test"));
}
