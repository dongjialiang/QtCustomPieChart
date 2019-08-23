# QtCustomPieChart

### Development Enviroment
* **Qt** 5.12.1
* **MinGW** 7.3.0 (Windows10)

![CustomPieChart][customPieChart]

### How does it work?
```c++
    #include "custompiechart.h" // 导入头文件
```
#### Create a custom pie chart widget.
```c++
    CustomPieChart * pieChart = new CustomPieChart(this); // CustomPieChart(parent)

    CustomPieChart * pieChart_1 = new CustomPieChart("逆变器31（台）", "小型", 18, Qt::blue, this); // CustomPieChart(titleName, tag, data, color, parent)

    QList<int> dataList = { 31, 25, 10 };
    QList<QString> tagList = { "正常", "离线", "异常" };
    QList<QColor> colorList = {QColor(57, 207, 206), QColor(232, 184, 232), Qt::yellow};

    CustomPieChart * pieChart_2 = new CustomPieChart("数据采集器1（台）", tagList, dataList, colorList, this); // CustomPieChart(titleName, tagList, dataList, colorList, parent)
```
#### Add slice.
```c++
    pieChart->addSlice(tag, data, color);
```
#### Set series.
```c++
    pieChart->setSeries(tagList, dataList, colorList);
```
#### Set global font.
```c++
    QFont tempFont;
    tempFont.setPointSize(9);
    tempFont.setFamily("Microsoft YaHei");
    tempFont.setWeight(80);
    pieChart->setGlobalFont(tempFont);
```
#### Set title font.
```c++
    pieChart->setTitleFont(tempFont);
```
#### Set tag font.
```c++
    pieChart->setTagFont(tempFont);
```
#### Set legend font.
```c++
    pieChart->setLegendFont(tempFont);
```
#### Set sum font.
```c++
    pieChart->setSumFont(tempFont);
```
#### Set "sum" text font.
```c++
    pieChart->setSumTextFont(tempFont);
```
#### Set ring size.
```c++
    pieChart->setRingSize(ringSize);
```
[customPieChart]: README/customPieChart.gif
