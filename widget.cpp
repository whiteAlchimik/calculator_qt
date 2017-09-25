#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    _gridLayout = new QGridLayout(this);
    _lcdNumber = new QLCDNumber;
    _ptrBtns = new QPushButton *[_numberOfButtons];
    for(size_t i = 0; i < _numberOfButtons; i++)
        _ptrBtns[i] = new QPushButton;

    configureLCDNumber();
    configureButtons();

    _gridLayout->addWidget(_lcdNumber, 0, 0, 2, 4);
    for(size_t i = 0; i < _rowWithBtn; i++)
        for(size_t j = 0; j < _columnWithBtn; j++)
            _gridLayout->addWidget(_ptrBtns[i * _columnWithBtn + j], i + 2,j);

    _displayString = '0';
}

Widget::~Widget()
{
    delete _ptrBtns;
}

void Widget::clickedButton()
{
    QString operation = qobject_cast<QPushButton *>(sender())->text();

    if(operation == "C")
    {
        cleaningOperation();
    }
    else if(operation == "<-")
    {
        deleteSymbolOperation();
    }
    else if(operation == "+/-")
    {
        changeSignNumberOperation();
    }
    else if(operation == "%" || operation == "/" || operation == "*" ||
            operation == "+" || operation == "-")
    {
        binaryOperation(_displayString, operation);
    }
    else if(operation == "=")
    {
        resultOperation();
    }
    else if(operation == '.')
    {
        dotOperation();
    }
    else if(operation >= "0" && operation <= "9")
    {
        inputSymbolOperation(operation);
    }

    _lcdNumber->display(_displayString);
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
    {
        inputSymbolOperation(QString(event->key()));
    }
    else if(event->key() == Qt::Key_C || event->key() == Qt::Key_Escape)
    {
        cleaningOperation();
    }
    else if(event->key() == Qt::Key_Period)
    {
        dotOperation();
    }
    else if(event->key() == Qt::Key_Plus)
    {
        binaryOperation(_displayString, "+");
    }
    else if(event->key() == Qt::Key_Minus)
    {
        binaryOperation(_displayString, "-");
    }
    else if(event->key() == Qt::Key_Asterisk)
    {
        binaryOperation(_displayString, "*");
    }
    else if(event->key() == Qt::Key_Slash)
    {
        binaryOperation(_displayString, "/");
    }
    else if(event->key() == Qt::Key_Return)
    {
        resultOperation();
    }

    _lcdNumber->display(_displayString);
}

void Widget::configureLCDNumber()
{
    _lcdNumber->setDecMode();
    _lcdNumber->setSmallDecimalPoint(true);
    _lcdNumber->setSegmentStyle(QLCDNumber::Filled);
    _lcdNumber->setDigitCount(numberOfDigits);
    _lcdNumber->setMinimumSize(240, 60);
}

void Widget::configureButtons()
{
    QString ptrStr[_rowWithBtn][_columnWithBtn] =
    {{"C", "<-", "+/-", "%"},
     {"7", "8", "9", "/"},
     {"4", "5", "6", "*"},
     {"1", "2", "3", "+"},
     {"0", ".", "=", "-"}};

    for(size_t i = 0; i < _rowWithBtn; i++)
    {
        for(size_t j = 0; j < _columnWithBtn; j++)
        {
            _ptrBtns[i * _columnWithBtn + j]->setText(ptrStr[i][j]);
            _ptrBtns[i * _columnWithBtn + j]->setMinimumSize(60, 30);
            QObject::connect(_ptrBtns[i * _columnWithBtn + j], SIGNAL(clicked()),
                    this, SLOT(clickedButton()));
        }
    }
}

void Widget::cleaningOperation()
{
    _displayString.clear();
    _displayString = '0';

    _ptrStack.clear();
}

void Widget::deleteSymbolOperation()
{
    size_t sizeStr = _displayString.size();
    if((sizeStr == 1) || (sizeStr == 2 && _displayString[0] == '-'))
    {
        cleaningOperation();
    }
    else
    {
        _displayString = _displayString.remove(sizeStr - 1, 1);
    }
}

void Widget::changeSignNumberOperation()
{
    double temp = _displayString.toDouble();
    _displayString = QString::number(temp * (-1));
}

void Widget::binaryOperation(const QString &number, const QString &operation)
{
    if(_ptrStack.isEmpty())
    {
        _ptrStack.push(number);
        _ptrStack.push(operation);
    }

    _displayString.clear();
    _displayString = '0';
}

void Widget::resultOperation()
{
    if(_ptrStack.size() != 2)
    {
        _ptrStack.clear();
        return;
    }

    QString operation = _ptrStack.pop();
    double firstArgument = _ptrStack.pop().toDouble();
    double secondArgument = _displayString.toDouble();

    if(operation == "%")
    {
        _displayString = QString::number(static_cast<int>(firstArgument) %
                                         static_cast<int>(secondArgument));
    }
    else if(operation == "/")
    {
        _displayString = QString::number(firstArgument / secondArgument);
    }
    else if(operation == "*")
    {
        _displayString = QString::number(firstArgument * secondArgument);
    }
    else if(operation == "+")
    {
        _displayString = QString::number(firstArgument + secondArgument);
    }
    else if(operation == "-")
    {
        _displayString = QString::number(firstArgument - secondArgument);
    }
}

void Widget::dotOperation()
{
    if(_displayString[_displayString.size() - 1] != '.')
        _displayString += '.';
}

void Widget::inputSymbolOperation(const QString &symbol)
{
    if(_displayString.size() < numberOfDigits)
    {
        if(_displayString.size() == 1 && _displayString[0] == '0')
            _displayString = symbol;
        else
            _displayString += symbol;
    }
}
