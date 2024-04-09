#include <log4cpp/Appender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>
using namespace log4cpp;

int main()
{
    // 输出器
    OstreamAppender *osAppender = new OstreamAppender("cout",&std::cout);
    FileAppender *fAppender = new FileAppender("file","file.log");
    RollingFileAppender *rfAppender = new RollingFileAppender("rf","rf.log",1 * 1024,3);
    // 格式化器
    PatternLayout * ptn1 = new PatternLayout();
    PatternLayout * ptn2 = new PatternLayout();
    PatternLayout * ptn3 = new PatternLayout();
    ptn1->setConversionPattern("%d %c [%p] %m%n");
    ptn2->setConversionPattern("%d %c [%p] %m%n");
    ptn3->setConversionPattern("%d %c [%p] %m%n");
    osAppender->setLayout(ptn1);
    fAppender->setLayout(ptn2);
    rfAppender->setLayout(ptn3);
    // 记录器
    Category &mylog = Category::getRoot().getInstance("mylog");
    // 过滤器
    mylog.setPriority(Priority::WARN);

    // 连接
    mylog.addAppender(osAppender);
    mylog.addAppender(fAppender);
    mylog.addAppender(rfAppender);

    // 测试
    for(int i = 0; i < 100; ++i){
        mylog.emerg("this is an emerg msg");
        mylog.fatal("this is a fatal msg");
        mylog.alert("this is an alert msg");
        mylog.crit("this is a crit msg");
        mylog.error("this is an error msg");
        mylog.warn("this is a warn msg");
        mylog.notice("this is a notice msg");
        mylog.info("this is an info msg");
    }

    // 回收资源
    Category::shutdown();
    return 0;
}

