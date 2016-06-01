#include "gamecontroller.h"
#include <iostream>

GameController::GameController(Helper* helper, VolumeControl *volume, QSqlDatabase *db,  QObject *parent)
    : QObject(parent), helper(helper), volume(volume), db(db)
{
    helper->setDot(QPointF(0.1, 0.1), QColor("green"));
    helper->setDot(QPointF(0.0, 0.0), QColor("blue"));
    helper->setDot(QPointF(0.25, 0.25), QColor("red"));
    //updateStatistics();
    connect(&stats_timer, SIGNAL(timeout()), this, SLOT(updateStatistics()));
    stats_timer.setSingleShot(false);
    stats_timer.setInterval( statistics_interval );
    stats_timer.start();
    computeGameMode();
}
void GameController::setSessionID(int session_id){
    this->session_id = session_id;
}

 void GameController::setData(qreal left_alpha, qreal  right_alpha, qreal  left_beta, qreal  right_beta){
     QPointF location;
     QColor color;
     QString title;
     qreal left_volume=0, right_volume=0;
     qreal x=0,y=0, n=0, enhance;
     int r=0,g=0,b=0;

     n = ((( left_beta + right_beta )/(left_beta+right_beta+left_alpha+right_alpha)));
     enhance = (( left_beta/(left_beta + right_beta) ) + ( right_alpha/(left_alpha + right_alpha))) ;

     switch(currentGameModeY){
     case NoGameY:
         y=0;
         title += QString(" No Game Y. ");
         break;
     case RightAlphaGreaterThanRightBeta:
         y = (( right_alpha / ( right_alpha + right_beta )));
         title += QString(" Y: RA>RB %1").arg(y*100, -5, 'f', 1,'0');
         break;
     case LeftBetaGreaterThanLeftAlpha:
         y = (( left_beta / ( left_beta + left_alpha )));
         title += QString(" Y: LB>LA %1").arg(y*100, -5, 'f', 1,'0');
         break;
     case FrontalBetaGreaterThanAlpha:
         y = ((( left_beta + right_beta )/(left_beta+right_beta+left_alpha+right_alpha)));
         title += QString(" Y: B>A %1").arg(y*100, -5, 'f', 1,'0');
         break;
     }

      switch(currentGameModeX){
      case NoGameX:
         title = QString(" No Game X ") + title;
          break;
      case RightAlphaGreaterThanLeft:
          x = (( right_alpha / (right_alpha + left_alpha))) ;
         title = QString(" X: RA>LA %1 ").arg(x*100, -5, 'f', 1,'0') + title;
          break;
      case LeftBetaGreaterThanRight:
          x = (( left_beta / (left_beta + right_beta ))) ;
         title = QString("X: LB>RB %1 ").arg(x*100, -5, 'f', 1,'0') + title;
          break;
      }

      if(currentGameModeY != NoGameY && currentGameModeX != NoGameX){
          if(x>0.5 && y>0.5)  score++;
          r = y >=0 ? y*255:0;
          g = x >=0 ? x*255:0;
          b = n >=0 ? n*255:0;
          left_volume = y;
          right_volume = x;
      }else if(currentGameModeY == NoGameY){
          y=0.5;
          left_volume = x;
          right_volume = x;
          if(x>0.5)  score++;
          g = x >=0 ? x*255:0;
          r = g;
          b = n >=0 ? n*255:0;
      }else if(currentGameModeX == NoGameX){
          x=0.5;
          left_volume = y;
          right_volume = y;
          if(y>0.5)  score++;
          r = y >=0 ? y*255:0;
          g = r;
          b = n >=0 ? n*255:0;
      }else{
          y=0.5;
          x=0.5;
          left_volume = .5;
          right_volume = .5;
          b = n >=0 ? n*255:0;
          r = b;
          g = b;
      }

     //std::cout << "rl_alpha:" << x << "lr_beta:" << y << "beta:" << n <<std::endl;
     location.setX( (x-.5) * 2.0);
     location.setY( (y-.5) * 2.0);
     color.setRed(r);
     color.setGreen(g);
     color.setBlue(b);
     left_volume = left_volume > 0 ? left_volume*100.0:0;
     right_volume = right_volume > 0 ? right_volume * 100.0:0;
     //std::cout << "rv: " << right_volume << "lv: "<<left_volume<<std::endl;

     helper->setScore(score);
     helper->setTitle(title);
     helper->setEnhance(enhance*100.0);
     helper->setDot(location, color);
     volume->setMasterAlsaVolume(left_volume,  right_volume);
 }

 void GameController::computeGameMode(){
     currentGameModeX = NoGameX;
     currentGameModeY = NoGameY;

     if( stat_raGrb < 25.0 ){
         currentGameModeY = RightAlphaGreaterThanRightBeta;
     }else if( stat_lbGla < 70.0){
         currentGameModeY = LeftBetaGreaterThanLeftAlpha;
     }else if( stat_bGa < 70.0){
         currentGameModeY = FrontalBetaGreaterThanAlpha;
     }else{
         if( stat_raGla < 30.0){
             currentGameModeX = RightAlphaGreaterThanLeft;
         }else if( stat_lbGrb < 60.0){
             currentGameModeX = LeftBetaGreaterThanRight;
         }else{
             currentGameModeX = RightAlphaGreaterThanLeft;
             currentGameModeY = FrontalBetaGreaterThanAlpha;
         }
     }
 }
 //     Date			 Count	 Goal%	 		LB>RB%	 	RA>LA%	 B>A%	 	LA%	 		RA%	 		LB%	 		RB%
 //    Grand Average	     		 12.6 		 64.7	 		48.9	 		41.7	 		25.3	 		25.6	 		26.4	 		22.7

 qreal GameController::clamp(qreal n){
     n = n>1.0 ? 1.0:n;
     n = n<-1.0 ? -1.0:n;
     return n;
 }
 void GameController::updateStatistics(){
     if(db->isOpen() && session_id > 0 ){
         q = QSqlQuery("", *db);
         QString qry;
         qry = QString("SELECT \
                       t.raGrb0/t.cnt*100 raGrb0, \
                       t.lbGla1/t.cnt*100 lbGla1, \
                       t.lbGrb2/t.cnt*100 lbGrb2, \
                       t.raGla3/t.cnt*100 raGla3, \
                       t.bGa4/t.cnt*100 bGa4, t.cnt cnt, t.ms ms  FROM \
                       (SELECT  \
                        SUM(CASE WHEN right_alpha > right_beta THEN 1 ELSE 0 END)  raGrb0, \
                        SUM(CASE WHEN  left_beta > left_alpha THEN 1 ELSE 0 END) lbGla1, \
                        SUM(CASE WHEN left_beta > right_beta THEN 1 ELSE 0 END)  lbGrb2, \
                        SUM(CASE WHEN right_alpha > left_alpha THEN 1 ELSE 0 END)  raGla3, \
                        SUM(CASE WHEN  (right_beta + left_beta) > (right_alpha + left_alpha) THEN 1 ELSE 0 END)  bGa4, \
                        count(*) cnt,  \
                        MAX(msecs) ms \
                        FROM alpha_beta  \
                        WHERE  session_id = %1 AND msecs > %2 ) t")
                       .arg(session_id).arg((int)(stat_ms));
                         qDebug() << qry;
     q.exec(qry);
     if(q.next()){
         stat_cnt    = q.value(5).toInt();
         if(stat_cnt>0){
             stat_raGrb = q.value(0).toReal();
             stat_lbGla = q.value(1).toReal();
             stat_lbGrb = q.value(2).toReal();
             stat_raGla = q.value(3).toReal();
             stat_bGa   = q.value(4).toReal();
             stat_ms     = q.value(6).toInt();
         }
     }
         q.finish();
     }
     std::cout << "cnt:" << stat_cnt << " B>A:" << stat_bGa << " RA>LA:" << stat_raGla
               << " LB>RB:" << stat_lbGrb << " RA>RB:" << stat_raGrb << " LB>LA:" << stat_lbGla << std::endl;
     computeGameMode();
}
