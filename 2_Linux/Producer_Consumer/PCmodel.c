#include <54func.h>
#define INITNUM 8
#define MAXNUM 10
//链表结点
typedef struct node_s
{
    int val;    //商品编号
    struct node_s *next;
}node_t;
//链表
typedef struct list_s
{
    int size;
    node_t *head;
    node_t *tail;
}list_t;
//共享资源
typedef struct shareRes_s
{
    list_t *list;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}shareRes_t;

//队列操作
//尾部入队
int dc_push(list_t *list,int val)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->next = NULL;
    node->val = val;
    ++list->size;
    if(list->head == NULL)
    {
        list->head = list->tail = node;
        return node->val;
    }
    //不是第一个节点
    list->tail->next = node;
    list->tail = node;
    return 0;
}
//头部出队
int dc_pop(list_t *list)
{
    node_t *temp = list->head;
    if(list->size == 1)
    {
        list->head = list->tail = NULL;
    }
    else
    {
        list->head = list->head->next;
    }
    --list->size;
    free(temp);
    return 0;
}
//遍历队列
int dc_visit(list_t *list)
{
    node_t *node = list->head;
    while(node)
    {
        printf("%d-->",node->val);
        node = node->next;
    }
    printf("NULL\n");
    return 0;
}

//生产者
void *producer(void *arg)
{
    shareRes_t *pshared = (shareRes_t *)arg;
    while(1)
    {
        sleep(3);
        //商品数量达到上限就wait
        pthread_mutex_lock(&pshared->mutex);
        while(pshared->list->size == MAXNUM)
        {
            pthread_cond_wait(&pshared->cond,&pshared->mutex);
        }
        int val = rand()%1000;
        dc_push(pshared->list,val);
        printf("生产一件编号为%d的商品，当前数量为：%d\n",val,pshared->list->size);
        dc_visit(pshared->list);
        pthread_cond_broadcast(&pshared->cond);
        pthread_mutex_unlock(&pshared->mutex);
    }
    pthread_exit(NULL);
}

//消费者
void *consumer(void *arg)
{
    shareRes_t *pshared = (shareRes_t *)arg;
    sleep(5);
    while(1)
    {
        sleep(1);
        //商品数量为0时wait
        pthread_mutex_lock(&pshared->mutex);
        while(pshared->list->size == 0)
        {
            pthread_cond_wait(&pshared->cond,&pshared->mutex);
        }
        int val = pshared->list->head->val;
        dc_pop(pshared->list);
        printf("消费一件编号为%d的商品，当前数量为：%d\n",val,pshared->list->size);
        dc_visit(pshared->list);
        pthread_cond_broadcast(&pshared->cond);
        pthread_mutex_unlock(&pshared->mutex);
    }
    pthread_exit(NULL);
}

int main()
{
    //初始化共享资源
    shareRes_t shared;
    shared.list = (list_t *)calloc(1,sizeof(list_t));
    pthread_mutex_init(&shared.mutex,NULL);
    pthread_cond_init(&shared.cond,NULL);
    //设置随机种子
    srand(time(NULL));
    //初始化队列
    for(int i = 0;i < INITNUM;++i)
    {
        int val = rand()%1000;
        dc_push(shared.list,val);
        printf("生产一件编号为%d的商品，当前数量为：%d\n",val,shared.list->size);
    }
    dc_visit(shared.list);

    //创建子线程
    pthread_t tidp1,tidp2,tidp3;
    pthread_t tidc1,tidc2;
    int ret = pthread_create(&tidp1,NULL,producer,&shared);
    THREAD_ERROR_CHECK(ret,"pthread_create_p1");
    ret = pthread_create(&tidp2,NULL,producer,&shared);
    THREAD_ERROR_CHECK(ret,"pthread_create_p2");
    ret = pthread_create(&tidp3,NULL,producer,&shared);
    THREAD_ERROR_CHECK(ret,"pthread_create_p3");
    ret = pthread_create(&tidc1,NULL,consumer,&shared);
    THREAD_ERROR_CHECK(ret,"pthread_create_c1");
    ret = pthread_create(&tidc2,NULL,consumer,&shared);
    THREAD_ERROR_CHECK(ret,"pthread_create_c2");

    //回收子线程
    pthread_join(tidp1,NULL);
    pthread_join(tidp2,NULL);
    pthread_join(tidp3,NULL);
    pthread_join(tidc1,NULL);
    pthread_join(tidc2,NULL);

    return 0;
}
//链表测试
//int main()
//{
//    list_t *list = (list_t *)calloc(1,sizeof(list_t));
//    for(int i = 0; i < 5; ++i)
//    {
//        dc_push(list);
//    }
//    node_t *plist = list->head;
//    while(plist)
//    {
//        printf("%d-->",plist->val);
//        plist = plist->next;
//    }
//    printf("NULL\n");
//    for(int i = 0; i < 5; ++i)
//    {
//        dc_pop(list);
//    }
//    return 0;
//}
