#include "custompiechart.h"

CustomPieChart::CustomPieChart(QWidget * parent)
    : QWidget (parent) {
    title = "默认标题"; // 标题名字
    total = 0;
    initPieChartWidget();
}
CustomPieChart::CustomPieChart(QString title,
                               QString tag, int data, QColor color, QWidget * parent)
    : QWidget (parent) {
    this->title = title; // 标题名字
    total = 1;           // 数据长度
    sum = data;          // 数据总量
    addSlice(tag,  data, color);
    initPieChartWidget();
}
CustomPieChart::CustomPieChart(QString title, QList<QString> tagList, QList<int> dataList,
                               QList<QColor> colorList, QWidget * parent)
    : QWidget (parent) {
    this->title = title; // 标题名字
    setSeries(tagList, dataList, colorList);
    initPieChartWidget();
}
CustomPieChart::~CustomPieChart() {}
/* 过滤绘制事件 */
bool CustomPieChart::eventFilter(QObject *widget, QEvent *event) {
    if (widget == pieChartWidget && event->type() == QEvent::Paint && total != 0) {
        drawPieChart(); // 绘制饼图部件
    }
    if (widget == titleWidget && event->type() == QEvent::Paint) {
        QPainter painter(titleWidget);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing); // 绘图和绘图文字抗锯齿
        painter.save();
        /* 绘制标题部件 */
        painter.setPen(Qt::white);

        titleFont.setPointSizeF(!isSetTitleFont ? titleWidget->height() * 0.75 : titleFont.pointSizeF());
        titleFont.setWeight(!isSetTitleFont ? QFont::Bold : titleFont.weight());
        painter.setFont(titleFont);
        painter.drawText(QRectF(0, 0, width(), height()), title);

        painter.restore();
    }
    return QWidget::eventFilter(widget, event);
}
/* 绘制饼图 */
void CustomPieChart::drawPieChart() {
    int width = this->width();
    int height = this->height();
    double min = qMin(width, height); // 宽和高中最小的值
    double diameter = min * 5 / 9;    // 直径
    double radius = diameter / 2;     // 半径
    int startLength = 0;              // 起始长度
    int midPoint = 0;                 // 坐标原点
    double startAngle = 0;            // 起始角度

    QPainter painter(pieChartWidget);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing); // 绘图和绘图文字抗锯齿
    painter.translate(width / 2, height / 2);                                    // 偏移起始点

    painter.save();

    fontSize = globalFont.pointSize();
    /* 绘制总数 */
    sumFont.setPointSize(!isSetSumFont ? int(radius / 2.5) : sumFont.pointSize());
    painter.setFont(sumFont);
    painter.setPen(QColor(54, 235, 171));
    painter.drawText(QRectF(-radius, -radius - (radius / 4), diameter, diameter),
                     Qt::AlignCenter, QString::number(sum));
    /* 绘制总数文本 */
    painter.setPen(Qt::white);
    painter.setFont(sumTextFont);
    painter.drawText(QRectF(-radius, -radius + fontSize, diameter, diameter), Qt::AlignCenter, "总数");

    painter.setFont(globalFont);
    for (int count = 0; count < total; count++) {
        startLength += (count > 0)
                ? int(360 * 16 * dataList[count - 1] / sum) : 0;
        int arcLength = int(360 * 16 * dataList[count] / sum); // 弧长
        double angle = 360 * dataList[count] / sum;            // 扇形的弧度
        startAngle += (count > 0)
                ? 360 * dataList[count - 1] / sum : 0;
        double radian = angle / 2 + startAngle; // 当前弧度(所在扇形的二等分角度 + 所在扇形的起始角度)
        double offset = radius / fontSize;      // 偏移量
        double offsetX = 0;                     // x轴偏移量
        double offsetY = 0;                     // y轴偏移量
        /* 绘制扇形区域 */
        painter.setPen(Qt::NoPen);
        QRadialGradient radialGradient(midPoint, midPoint, radius);
        radialGradient.setColorAt(0, Qt::transparent);
        radialGradient.setColorAt(ringSize, Qt::transparent);
        radialGradient.setColorAt((ringSize + 0.01 > 1) ? 1 : ringSize + 0.01,
                                  qFuzzyCompare(ringSize, 1)
                                  ? Qt::transparent : colorList[count]); // 从颜色列表中取出颜色并设为区域背景色
        radialGradient.setColorAt(1, qFuzzyCompare(ringSize, 1)
                                  ? Qt::transparent : colorList[count]);
        painter.setBrush(radialGradient);
        painter.drawPie(QRectF(-radius, -radius, diameter, diameter), startLength, arcLength); // 画饼图的扇形区域
        /* 绘制说明 */
        painter.setPen(colorList[count]);
        painter.setBrush(colorList[count]);
        painter.setFont(legendFont);
        fontSize = legendFont.pointSize();
        painter.drawRect(QRectF(-(radius * 2.3), -(radius / 2) + count * (fontSize + 6), 5, 5));
        painter.drawText(QRectF(-(radius * 2.1), -(radius / 1.8) + count * (fontSize + 6), radius, radius), tagList[count]);
        /* 绘制标签折线 */
        painter.setFont(tagFont);
        fontSize = tagFont.pointSize();
        QPointF point = QPointF(midPoint + radius * cos(radian * M_PI / 180),
                                midPoint - radius * sin(radian * M_PI / 180)); // 弧度在坐标轴的象限点
        QPolygonF polygon; // 多段线
        polygon << point;
        QString tagText = QString::number(dataList[count] / sum * 100, 'f', 0) + "%";
        double textWidth = tagText.size() / 2 * fontSize;
        if (dataList[count] != 0) { // 数据不为零才可以绘制折线和标签
            if (radian > 0 && radian <= 90) {          // 第一象限
                offsetX += offset;
                offsetY -= offset;
                polygon << QPointF(point.x() + offsetX, point.y() + offsetY);
                offsetX += textWidth;
                polygon << QPointF(point.x() + offsetX, point.y() + offsetY);
                offsetX -= textWidth;
                offsetY -= (fontSize * 1.5);
            } else if (radian > 90 && radian <= 180) { // 第二象限
                offsetX -= offset;
                offsetY -= offset;
                polygon << QPointF(point.x() + offsetX, point.y() + offsetY);
                offsetX -= textWidth;
                polygon << QPointF(point.x() + offsetX, point.y() + offsetY);
                offsetY -= (fontSize * 1.5);
            } else if (radian > 180 && radian <= 270) { // 第三象限
                offsetX -= offset;
                offsetY += offset;
                polygon << QPointF(point.x() + offsetX, point.y() + offsetY);
                offsetX -= textWidth;
                polygon << QPointF(point.x() + offsetX, point.y() + offsetY);
            } else if (radian > 270 && radian <= 360) { // 第四象限
                offsetX += offset;
                offsetY += offset;
                polygon << QPointF(point.x() + offsetX, point.y() + offsetY);
                offsetX += textWidth;
                polygon << QPointF(point.x() + offsetX, point.y() + offsetY);
                offsetX -= textWidth;
            }
            painter.drawPolyline(polygon);
            /* 绘制标签 */
            painter.drawText(QRectF(point.x() + offsetX, point.y() + offsetY,
                                    diameter, diameter), QStringLiteral("%1").arg(tagText));
        }
    }
    painter.restore();
}
/* 初始化饼图部件 */
void CustomPieChart::initPieChartWidget() {
    QFont tempFont;
    tempFont.setPointSize(10);
    setGlobalFont(tempFont);
    isSetSumFont = false;
    isSetTitleFont = false;

    setRingSize(0.6);
    /* 标题部件 */
    titleWidget = new QWidget(this);
    titleWidget->installEventFilter(this);
    /* 饼图部件 */
    pieChartWidget = new QWidget(this);
    pieChartWidget->installEventFilter(this);

    QVBoxLayout * vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setSpacing(0);
    vBoxLayout->setMargin(0);
    vBoxLayout->addWidget(titleWidget, 1);
    vBoxLayout->addWidget(pieChartWidget, 9);
    setLayout(vBoxLayout);
}
/* 增加切片 */
void CustomPieChart::addSlice(QString tag, int data, QColor color) {
    tagList << tag;          // 标签名列表
    dataList << data;        // 数据列表
    colorList << color;      // 颜色列表
    total = dataList.size(); // 数据表长度
    sum += (total == 1) ? 0 : dataList[total - 1];
}
/* 设置系列 */
void CustomPieChart::setSeries(QList<QString> tagList, QList<int> dataList, QList<QColor> colorList) {
    total = dataList.size();     // 数据表长度
    sum = 0;                     // 数据总量
    this->tagList = tagList;     // 标签名列表
    this->dataList = dataList;   // 数据列表
    this->colorList = colorList; // 颜色列表
    for (int count = 0; count < total; count++) {
        sum += dataList[count];
    }
}
/* 设置全局字体 */
void CustomPieChart::setGlobalFont(QFont font) {
    globalFont = font;
    titleFont = font;
    isSetTitleFont = true;
    tagFont = font;
    legendFont = font;
    sumFont = font;
    sumTextFont = font;
    sumFont = font;
    isSetSumFont = true;
}
/* 设置标题字体 */
void CustomPieChart::setTitleFont(QFont font) {
    titleFont = font;
    isSetTitleFont = true;
}
/* 设置标签字体 */
void CustomPieChart::setTagFont(QFont font) {
    tagFont = font;
}
/* 设置说明字体 */
void CustomPieChart::setLegendFont(QFont font) {
    legendFont = font;
}
/* 设置总数字体 */
void CustomPieChart::setSumFont(QFont font) {
    sumFont = font;
    isSetSumFont = true;
}
/* 设置"总数"文本字体 */
void CustomPieChart::setSumTextFont(QFont font) {
    sumTextFont = font;
}
/* 设置圆环大小 */
void CustomPieChart::setRingSize(double ringSize) {
    this->ringSize = (ringSize > 1) ? 1 : ringSize;
}
