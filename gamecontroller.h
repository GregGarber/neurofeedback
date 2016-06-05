#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QtSql>
#include "helper.h"
#include "volumecontrol.h"

struct EEGFreq ;
struct EEGChannel;
struct EEGData;

class Helper;
class VolumeControl;
class GameController : public QObject
{
    Q_OBJECT
public:
    explicit GameController(Helper* helper, VolumeControl* volume,QSqlDatabase* db, QObject *parent = 0);
    void setData();
    void setEEGData(EEGData &data);
    qreal clamp(qreal n);
    void setSessionID(int session_id);
    void computeGameMode();
    EEGData* eeg_data;
    enum GameModeX {
        RaiseRightAlphaToLeft ,
        RaiseRightAlphaToRightBeta,
        LowerFrontalAlpha,
        RaiseRightAlpha
    };
    enum GameModeY {
        RaiseLeftBetaToRightAlpha,
        RaiseLeftBetaToRight,
        RaiseFrontalBetaToAlpha,
        RaiseLeftBetaToLeftAlpha,
        LowerRightBeta,
        RaiseLeftBeta
    };
    GameModeX  currentGameModeX = RaiseRightAlpha;
    GameModeY  currentGameModeY = RaiseLeftBeta;

signals:

public slots:

private:
    long score = 0;
    int session_id = 77;
    const qreal statistics_interval = 1000 * 60 * 2;
    qreal stat_lbGrb=.3, stat_raGla=.3, stat_bGa=.3, stat_raGrb=.3, stat_lbGla=.3, stat_lbGra = .3;
    qreal stat_lam = .3, stat_lbm = .3,stat_ram = .3,stat_rbm = .3;
    int stat_cnt=0,  stat_ms=0;
    Helper* helper;
    VolumeControl* volume;
    QSqlDatabase* db;
    QSqlQuery q;
};

#endif // GAMECONTROLLER_H
