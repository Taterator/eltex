#include "../unity/unity.h"
#include "../src/queue.h"

queue q;
void setUp(void)
{
  initialize_queue(&q);
}
void tearDown(void)
{
  delete_all(&q);
}

int list_len(node_t *head)
{
  int count = 0;
  while (head)
  {
    count++;
    head = head->next;
  }
  return count;
}

void test_insert_single(void)
{
  add_node(&q, 5, "danger");
  TEST_ASSERT_NOT_NULL(q.head);
  TEST_ASSERT_NOT_NULL(q.tail);
  TEST_ASSERT_EQUAL_INT(5, q.head->priority);
  TEST_ASSERT_EQUAL_STRING("danger", q.head->message);
}

void test_insert_multiple_ordered(void)
{
  int priority[5] = {1, 2, 3, 4, 5};
  char *msg[5] = {"1", "2", "3", "4", "5"};
  for (int i = 0; i < 5; i++)
  {
    add_node(&q, priority[i], msg[i]);
  }
  TEST_ASSERT_NOT_NULL(q.head);
  TEST_ASSERT_NOT_NULL(q.tail);
  TEST_ASSERT_EQUAL_INT(5, list_len(q.head));
  node_t *cur_node = q.head;
  for (int i = 0; i < 5; i++)
  {
    TEST_ASSERT_EQUAL_INT(priority[i], cur_node->priority);
    TEST_ASSERT_EQUAL_STRING(msg[i], cur_node->message);
    cur_node = cur_node->next;
  }
}

void test_insert_multiple_unordered(void)
{
  int priority_og[5] = {4, 1, 2, 5, 3};
  char *msg_og[5] = {"4", "1", "2", "5", "3"};

  int priority[5] = {1, 2, 3, 4, 5};
  char *msg[5] = {"1", "2", "3", "4", "5"};

  for (int i = 0; i < 5; i++)
  {
    add_node(&q, priority_og[i], msg_og[i]);
  }
  TEST_ASSERT_NOT_NULL(q.head);
  TEST_ASSERT_NOT_NULL(q.tail);
  TEST_ASSERT_EQUAL_INT(5, list_len(q.head));
  node_t *cur_node = q.head;
  for (int i = 0; i < 5; i++)
  {
    TEST_ASSERT_EQUAL_INT(priority[i], cur_node->priority);
    TEST_ASSERT_EQUAL_STRING(msg[i], cur_node->message);
    cur_node = cur_node->next;
  }
}

void test_pop_head(void)
{
  add_node(&q, 5, "danger");
  TEST_ASSERT_NOT_NULL(q.head);
  TEST_ASSERT_NOT_NULL(q.tail);
  TEST_ASSERT_EQUAL_INT(5, q.head->priority);
  TEST_ASSERT_EQUAL_STRING("danger", q.head->message);
  node_t node;
  int res = pop(&q, &node);
  TEST_ASSERT_EQUAL_INT(0, res);
  TEST_ASSERT_EQUAL_INT(5, node.priority);
  TEST_ASSERT_EQUAL_STRING("danger", node.message);
  TEST_ASSERT_NULL(q.head);
  TEST_ASSERT_NULL(q.tail);
}

void test_pop_null(void)
{
  TEST_ASSERT_NULL(q.head);
  TEST_ASSERT_NULL(q.tail);
  node_t node;
  int res = pop(&q, &node);
  TEST_ASSERT_EQUAL_INT(1, res);
}

void test_pop_priority(void)
{
  int priority[5] = {1, 2, 3, 4, 5};
  char *msg[5] = {"1", "2", "3", "4", "5"};
  for (int i = 0; i < 5; i++)
  {
    add_node(&q, priority[i], msg[i]);
  }
  TEST_ASSERT_NOT_NULL(q.head);
  TEST_ASSERT_NOT_NULL(q.tail);
  TEST_ASSERT_EQUAL_INT(5, list_len(q.head));
  node_t node;
  int res = pop_message_by_priority(&q, 4, 2, &node);

  TEST_ASSERT_EQUAL_INT(0, res);
  TEST_ASSERT_EQUAL_INT(4, node.priority);
  TEST_ASSERT_EQUAL_STRING("4", node.message);
}

void test_pop_priority_fail(void)
{
  int priority[5] = {1, 2, 3, 4, 5};
  char *msg[5] = {"1", "2", "3", "4", "5"};
  for (int i = 0; i < 5; i++)
  {
    add_node(&q, priority[i], msg[i]);
  }
  TEST_ASSERT_NOT_NULL(q.head);
  TEST_ASSERT_NOT_NULL(q.tail);
  TEST_ASSERT_EQUAL_INT(5, list_len(q.head));
  node_t node;
  int res = pop_message_by_priority(&q, 6, 2, &node);

  TEST_ASSERT_EQUAL_INT(1, res);
}

void test_pop_priority_closest(void)
{
  int priority[5] = {1, 2, 3, 4, 10};
  char *msg[5] = {"1", "2", "3", "4", "10"};
  for (int i = 0; i < 5; i++)
  {
    add_node(&q, priority[i], msg[i]);
  }
  TEST_ASSERT_NOT_NULL(q.head);
  TEST_ASSERT_NOT_NULL(q.tail);
  TEST_ASSERT_EQUAL_INT(5, list_len(q.head));
  node_t node;
  int res = pop_message_by_priority(&q, 6, 1, &node);

  TEST_ASSERT_EQUAL_INT(0, res);
  TEST_ASSERT_EQUAL_INT(10, node.priority);
  TEST_ASSERT_EQUAL_STRING("10", node.message);
}

int main()
{
  UNITY_BEGIN();
  RUN_TEST(test_insert_single);
  RUN_TEST(test_insert_multiple_ordered);
  RUN_TEST(test_insert_multiple_unordered);
  RUN_TEST(test_pop_head);
  RUN_TEST(test_pop_null);
  RUN_TEST(test_pop_priority);
  RUN_TEST(test_pop_priority_fail);
  RUN_TEST(test_pop_priority_closest);

  return UNITY_END();
}