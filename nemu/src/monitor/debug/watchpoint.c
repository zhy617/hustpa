#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp() {
  // 检查是否有空闲的监视点
  Assert(free_ != NULL, "No free watchpoints available.");

  // 从 free_ 链表头部取出一个
  WP *wp = free_;
  free_ = free_->next;

  // 将其插入到 head 链表的头部
  wp->next = head;
  head = wp;

  return wp;
}

void free_wp(WP *wp) {
  if (wp == NULL) return;

  // 从 head 链表中移除 wp
  if (head == wp) {
    head = wp->next;
  } else {
    WP *p = head;
    while (p && p->next != wp) {
      p = p->next;
    }
    // 如果没找到，说明wp不在head链表中，直接返回
    if (p == NULL) return;
    p->next = wp->next;
  }

  // 将 wp 归还到 free_ 链表的头部
  wp->next = free_;
  free_ = wp;
  // 清空表达式内容
  wp->expr[0] = '\0';
  wp->last_value = 0;
}