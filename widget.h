#ifndef WIDGET_H
#define WIDGET_H

#include <QDebug>

#include <QWidget>
#include <QString>
#include <QStack>
#include <QGridLayout>
#include <QPushButton>
#include <QLCDNumber>
#include <QKeyEvent>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void clickedButton();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QGridLayout *_gridLayout;
    QLCDNumber *_lcdNumber;
    QPushButton **_ptrBtns;

    const size_t _numberOfButtons = 20;
    const static size_t _rowWithBtn = 5;
    const static size_t _columnWithBtn = 4;

    const size_t numberOfDigits = 12;

    QString _displayString;
    QStack<QString> _ptrStack;

    void configureLCDNumber();
    void configureButtons();

    void cleaningOperation();
    void deleteSymbolOperation();
    void changeSignNumberOperation();
    void binaryOperation(const QString &number, const QString &operation);
    void resultOperation();
    void dotOperation();
    void inputSymbolOperation(const QString &symbol);
};

#endif // WIDGET_H
