#include "helper.h"
#include <QDebug>

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>

#include <iostream>
#include <math.h>

Helper::Helper()
{
    background = QBrush(QColor(0, 0, 0,127));
    redBrush = QBrush(QColor(0xff,0,0));
    //textPen = QPen(Qt::black);
    //textPen = QPen(Qt::white);
    textPen = QPen(QColor(255,255,255,254));
    textFont.setPixelSize(30);

    //QImage i = QImage(QSize(400,400), QImage::Format_Alpha8 );
    QImage i;

    depression = QImage("depression.png" );
    depression.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    anxiety= QImage("anxiety.png");
    anxiety.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    mania= QImage("mania.png");
    mania.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    buddha= QImage("buddha.png");
    buddha.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    setTitle("This is my long ass title");
   //i.sharedPainter()->drawImage(0,0,mania);
    //background = QBrush(mania);
}

void Helper::paint(QPainter *painter, QPaintEvent *event, int elapsed)
{
    painter->save();

    qreal width, height, half_width, half_height;
    width = painter->device()->width();
    height = painter->device()->height();
    half_width = width/2.0;
    half_height = height/2.0;
    painter->translate(half_width, half_height);

    painter->drawImage(0,0, depression);
    painter->drawImage(-half_width, 0, anxiety);
    painter->drawImage(-half_width, -half_height, mania);
    painter->drawImage(0, -half_height, buddha);

    painter->setBrush(redBrush);
    painter->drawLine(-half_width, 0, half_width, 0);
    painter->drawLine(0,-half_height,0, half_height);

    painter->setPen(textPen);
    painter->fillRect(-half_width,-15, width,30, background);
    painter->setFont(textFont);
    QString score_str = QString("%1 %2 %3").arg(title).arg(enhance,-5, 'f', 1, '0').arg((int)score,7,10, QChar('0'));
    painter->drawText(QRect(-half_width, -15, width, 30), Qt::AlignCenter, score_str);

    Dot dot;
    int opacity;
    for(unsigned int i=0; i<dot_history.size(); i++){
        opacity = 55+((200/dot_history.size())*i);
        dot = dot_history[i];
        dotBrush = QBrush(QColor(dot.fill_color.red(), dot.fill_color.green(), dot.fill_color.blue(),opacity  ));
        dotPen = QPen(QColor(dot.fill_color.red(), dot.fill_color.green(), dot.fill_color.blue(), opacity ));
        painter->setBrush(dotBrush);
        painter->setPen(dotPen);
        painter->drawEllipse(QRectF(half_width * dot.location.rx(),  -half_height * dot.location.ry(),50,50));
    }
    painter->restore();
}

void Helper::setScore( long int score ){
    this->score = score;
}

void Helper::setTitle(QString title){
    this->title = title;
}

void Helper::setDot(QPointF location, QColor color){
    Dot dot;
    dot.location = location;
    dot.fill_color = color;
    dot_history.push_back(dot);
    if(dot_history.size() > HISTORY_LENGTH){
        dot_history.pop_front();
    }
}

void Helper::setEnhance(qreal enhance){
    this->enhance = enhance;
}

/*
void Helper::setData(qreal left_alpha, qreal  left_beta, qreal  right_alpha, qreal  right_beta){
    DotData dot;
    dot.x = (left_beta - right_beta) * 400.0;
    dot.y = (left_alpha - right_alpha) * 400.0;
        if(left_beta > left_alpha){
            dot.red = 128 + (left_beta - left_alpha) * 127 *2;
            dot.red = dot.red > 255 ? 255:dot.red;
        }else{
            dot.red = 128;
        }
        if(right_alpha > right_beta){
            dot.green = 128 + (right_alpha - right_beta) * 127*2;
            dot.green = dot.green > 255 ? 255:dot.green;
        }else{
            dot.green = 128.0;
        }

        if( (left_beta + right_beta) > (left_alpha + right_alpha)){
            dot.blue = 128 + ((left_beta + right_beta) - (left_alpha + right_alpha)) * 127*1;
            dot.blue = dot.blue > 255 ? 255:dot.blue;
        }else{
            dot.blue = 128;
        }
     dot.enhance  = (( left_beta/(left_beta + right_beta) ) + ( right_alpha/(left_alpha + right_alpha)) )*100.0;
    history.push_back(dot);
    if(history.size() > HISTORY_LENGTH){
        history.pop_front();
    }
}
*/
