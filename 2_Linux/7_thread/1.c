#include <54func.h>

typedef struct node_s
{
    struct node_s *next;
    int date;
}node_t;

void *threadFunc(void *arg)
{
    node_t *list = (node_t *)arg;
    while(list != NULL)
    {
        printf("%d-->",list->date);
        list = list->next;
    }
    printf("NULL\n");
    return NULL;
}

int main()
{
    node_t *list = NULL;
    node_t *node;
    for(int i = 0;i < 5;i++)
    {
        node = (node_t *)calloc(1,sizeof(node_t));
        ERROR_CHECK(node,NULL,"calloc");
        node->date = i;
        node->next = list;
        list = node;
    }
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,threadFunc,list);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    sleep(2);
    return 0;
}

