#ifndef CUSTOMPIECHART_H
#define CUSTOMPIECHART_H

#include <QPainter>
#include <QPaintEvent>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QtMath>

class CustomPieChart : public QWidget {
    Q_OBJECT

public:
    explicit CustomPieChart(QWidget * parent = nullptr);
    explicit CustomPieChart(const QString &title, const QString &tag, const int &data,
                            const QColor &color, QWidget * parent = nullptr);
    explicit CustomPieChart(const QString &title, QStringList tagList, QList<int> dataList,
                            QList<QColor> colorList, QWidget * parent = nullptr);
    ~CustomPieChart();
    void addSlice(const QString &tag, const int &data, const QColor &color);                                // 增加切片的函数
    void setSeries(QStringList tagList, QList<int> value, QList<QColor> colorList); // 设置系列的函数
    void setTitleFont(const QFont &font);                                                     // 设置标题字体的函数
    void setTagFont(const QFont &font);                                                       // 设置标签字体的函数
    void setLegendFont(const QFont &font);                                                    // 设置说明字体的函数
    void setSumFont(const QFont &font);                                                       // 设置总数字体的函数
    void setSumTextFont(const QFont &font);                                                   // 设置"总数"文本字体的函数
    void setGlobalFont(const QFont &font);                                                    // 设置全局字体的函数
    void setRingSize(const double &ringSize);                                                 // 设置圆环大小的函数

private:
    int total;                                          // 总的标签数
    double sum;                                         // 标签的总量
    QFont globalFont;                                   // 全局字体
    QFont titleFont;                                    // 标题字体
    QFont tagFont;                                      // 标签字体
    QFont legendFont;                                   // 说明字体
    QFont sumFont;                                      // 总数文本字体
    QFont sumTextFont;                                  // "总数"文本字体
    bool isSetTitleFont;                                // 判断标题是否设置字体
    bool isSetTagFont;                                  // 判断标签是否设置字体
    bool isSetLegendFont;                               // 判断说明是否设置字体
    bool isSetSumFont;                                  // 判断总数是否设置字体
    bool isSetSumTextFont;                              // 判断"总数"是否设置字体
    double fontSize;                                    // 字体大小
    double ringSize;                                    // 圆环
    QWidget * titleWidget;                              // 标题部件
    QWidget * pieChartWidget;                           // 绘制饼图的部件
    QString title;                                      // 标题名字
    QStringList tagList;                                // 标签列表
    QList<int> dataList;                                // 数据列表
    QList<QColor> colorList;                            // 颜色列表
    bool eventFilter(QObject * widget, QEvent * event); // 过滤绘图事件的函数
    void drawPieChart();                                // 绘制饼图的函数
    void initPieChartWidget();                          // 初始化饼图部件的函数

};

#endif // CUSTOMPIECHART_H
