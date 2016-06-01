#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QtSql>
#include "helper.h"
#include "volumecontrol.h"

class Helper;
class VolumeControl;
class GameController : public QObject
{
    Q_OBJECT
public:
    explicit GameController(Helper* helper, VolumeControl* volume,QSqlDatabase* db, QObject *parent = 0);
    void setData(qreal left_alpha, qreal  right_alpha, qreal  left_beta, qreal  right_beta);
    qreal clamp(qreal n);
    void setSessionID(int session_id);
    void computeGameMode();
    enum GameModeX {
        NoGameX = 0 ,
        LeftBetaGreaterThanRight = 1 ,
        RightAlphaGreaterThanLeft = 2 ,
        LeftBetaRightAlpha = 4
    };
    enum GameModeY {
        NoGameY = 0,
        FrontalBetaGreaterThanAlpha = 1,
        LeftBetaGreaterThanLeftAlpha = 2,
        RightAlphaGreaterThanRightBeta = 4
    };
    GameModeX currentGameModeX = NoGameX;
    GameModeY currentGameModeY = NoGameY;

signals:

public slots:
    void updateStatistics();

private:
    long score = 0;
    int session_id = 77;
 //                                     All
//     Date			 Count	 Goal%	 		LB>RB%	 	RA>LA%	 B>A%	 	LA%	 		RA%	 		LB%	 		RB%
//    Grand Average	     		 12.6 		 64.7	 		48.9	 		41.7	 		25.3	 		25.6	 		26.4	 		22.7
    const qreal statistics_interval = 1000 * 60 * 2;
    //const qreal statistics_interval = 1000 * 1;
    qreal stat_lbGrb=64.7, stat_raGla=48.9, stat_bGa=41.7, stat_raGrb=5.0, stat_lbGla=5;
    int stat_cnt=0,  stat_ms=0;
    Helper* helper;
    VolumeControl* volume;
    QSqlDatabase* db;
    QSqlQuery q;
    QTimer stats_timer;
};

#endif // GAMECONTROLLER_H
