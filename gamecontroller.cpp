#include "gamecontroller.h"
#include <iostream>
#include "window.h"
GameController::GameController(Helper* helper, VolumeControl *volume, QSqlDatabase *db,  QObject *parent)
        : QObject(parent), helper(helper), volume(volume), db(db)
{
    helper->setDot(QPointF(0.1, 0.1), QColor("green"));
    helper->setDot(QPointF(0.0, 0.0), QColor("blue"));
    helper->setDot(QPointF(0.25, 0.25), QColor("red"));
    computeGameMode();
}
void GameController::setEEGData(EEGData &data){
    this->eeg_data = &data;
}

void GameController::setSessionID(int session_id){
    this->session_id = session_id;
}

void GameController::setData(){
    if(!(
                            eeg_data->left.alpha.current > 0 &&
                            eeg_data->right.alpha.current > 0 &&
                            eeg_data->left.beta.current > 0 &&
                            eeg_data->right.beta.current > 0)
                    ) return;
    QPointF location;
    QColor color;
    QString title;
    qreal left_volume=0, right_volume=0;
    qreal x=0,y=0, n=0, enhance;
    int r=0,g=0,b=0;

    n = ((( eeg_data->left.beta.current + eeg_data->right.beta.current )/(eeg_data->left.beta.current+eeg_data->right.beta.current+eeg_data->left.alpha.current+eeg_data->right.alpha.current)));
    enhance = (( eeg_data->left.beta.current/(eeg_data->left.beta.current + eeg_data->right.beta.current) ) + ( eeg_data->right.alpha.current/(eeg_data->left.alpha.current + eeg_data->right.alpha.current))) ;

    switch(currentGameModeY){
    case RaiseLeftBeta:
        y = eeg_data->left.beta.current/ (eeg_data->left.beta.current + eeg_data->left.beta.mean);
        title += QString(" Y: LB Low %1").arg(y*100, -5, 'f', 1,'0');
        break;
    case LowerRightBeta:
        y = 1 -  (eeg_data->right.beta.current /(eeg_data->right.beta.current + eeg_data->right.beta.mean));
        title += QString(" Y: RB High %1").arg(y*100, -5, 'f', 1,'0');
        break;
    case RaiseLeftBetaToRightAlpha:
        y = (( eeg_data->left.beta.current / ( eeg_data->left.beta.current + eeg_data->right.alpha.current )));
        title += QString(" Y: LB>LA %1").arg(y*100, -5, 'f', 1,'0');
        break;
    case RaiseLeftBetaToLeftAlpha:
        y = (( eeg_data->left.beta.current / ( eeg_data->left.beta.current + eeg_data->left.alpha.current )));
        title += QString(" Y: LB>LA %1").arg(y*100, -5, 'f', 1,'0');
        break;
    case RaiseFrontalBetaToAlpha:
        y = ((( eeg_data->left.beta.current + eeg_data->right.beta.current )/(eeg_data->left.beta.current+eeg_data->right.beta.current+eeg_data->left.alpha.current+eeg_data->right.alpha.current)));
        title += QString(" Y: B>A %1").arg(y*100, -5, 'f', 1,'0');
        break;
    case RaiseLeftBetaToRight:
        y = (( eeg_data->left.beta.current / (eeg_data->left.beta.current + eeg_data->right.beta.current ))) ;
        title = QString("X: LB>RB %1 ").arg(y*100, -5, 'f', 1,'0') + title;
        break;
    }
    //@eeg_data->raGrb,                 @eeg_data->lbGla,                   @eeg_data->lbGrb ,                @eeg_data->raGla
    //0.48048442210917675, 0.39422553043551267, 0.5996045532740645, 0.4869192163546906
    // @eeg_data->left.beta.mean:=avg(eeg_data->left.beta.current), @eeg_data->left.alpha.mean:=avg(eeg_data->left.alpha.current), @eeg_data->right.beta.mean:=avg(eeg_data->right.beta.current), @eeg_data->right.alpha.mean:=avg(eeg_data->right.alpha.current)
    //  0.49058866672542767,       0.3192649857042121,           0.32759836011856586,         0.3029859265905634
    switch(currentGameModeX){
    case RaiseRightAlpha:
        x= eeg_data->right.alpha.current/(eeg_data->right.alpha.current + eeg_data->right.alpha.mean);
        title += QString(" Y: LB Low %1").arg(x*100, -5, 'f', 1,'0');
        break;
    case LowerFrontalAlpha:
        x= 1.0 -  ((eeg_data->left.alpha.current + eeg_data->right.alpha.current)/(eeg_data->left.alpha.current + eeg_data->left.beta.current + eeg_data->right.alpha.current + eeg_data->right.beta.current));
        title += QString(" Y: LB Low %1").arg(x*100, -5, 'f', 1,'0');
        break;
    case RaiseRightAlphaToRightBeta:
        x = (( eeg_data->right.alpha.current / ( eeg_data->right.alpha.current + eeg_data->right.beta.current )));
        title += QString(" Y: RA>RB %1").arg(x*100, -5, 'f', 1,'0');
        break;
    case RaiseRightAlphaToLeft:
        x = (( eeg_data->right.alpha.current / (eeg_data->right.alpha.current + eeg_data->left.alpha.current))) ;
        title = QString(" X: RA>LA %1 ").arg(x*100, -5, 'f', 1,'0') + title;
        break;
    }
    if(x>0.5 && y>0.5)  score++;

    r = y >=0 ? y*255:0;
    g = x >=0 ? x*255:0;
    b = n >=0 ? n*255:0;

    left_volume = y;
    right_volume = x;

    location.setX( (x-.5) * 3.0);
    location.setY( (y-.5) * 3.0);
    color.setRed(r);
    color.setGreen(g);
    color.setBlue(b);
    left_volume = left_volume > 0 ? left_volume*100.0:0;
    right_volume = right_volume > 0 ? right_volume * 100.0:0;

    helper->setScore(score);
    helper->setTitle(title);
    helper->setEnhance(enhance*100.0);
    helper->setDot(location, color);
    volume->setMasterAlsaVolume(left_volume,  right_volume);
}

void GameController::computeGameMode(){
    currentGameModeX = RaiseRightAlpha;
    currentGameModeY = RaiseLeftBeta;

    if( eeg_data->left.beta.mean < 0.49058866672542767 ){
        currentGameModeY = RaiseLeftBeta;
    }else if( eeg_data->right.beta.mean > 0.32759836011856586){
        currentGameModeY = LowerRightBeta;
    }else if(eeg_data->lbGra < 0.6182010750564462 ){
        currentGameModeY = RaiseLeftBetaToRightAlpha;
    }else if( eeg_data->lbGrb < 0.5996045532740645){
        currentGameModeY = RaiseLeftBetaToRight;
    }else if( eeg_data->lbGla < 0.39422553043551267){
        currentGameModeY = RaiseLeftBetaToLeftAlpha;
    }else if( eeg_data->bGa <  0.5680126887890662 ){
        currentGameModeY = RaiseFrontalBetaToAlpha;
    }else{
        currentGameModeY = RaiseLeftBeta;
    }
    //@eeg_data->raGrb,                 @eeg_data->lbGla,                   @eeg_data->lbGrb ,                @eeg_data->raGla
    //0.48048442210917675, 0.39422553043551267, 0.5996045532740645, 0.4869192163546906
    // @eeg_data->left.beta.mean:=avg(left_beta), @eeg_data->left.alpha.mean:=avg(left_alpha), @eeg_data->right.beta.mean:=avg(right_beta), @eeg_data->right.alpha.mean:=avg(right_alpha)
    //  0.49058866672542767,       0.3192649857042121,           0.32759836011856586,         0.3029859265905634

    if( eeg_data->right.alpha.mean <  0.3029859265905634){
        currentGameModeX = RaiseRightAlpha;
    }else if( eeg_data->raGla < 0.4869192163546906 ){
        currentGameModeX = RaiseRightAlphaToLeft;
    }else if( eeg_data->raGrb < 0.48048442210917675 ){
        currentGameModeX = RaiseRightAlphaToRightBeta;
    }else if( (eeg_data->left.alpha.mean + eeg_data->right.alpha.mean ) > (0.3192649857042121 + 0.3029859265905634)  ){
        currentGameModeX = LowerFrontalAlpha;
    }else{
        currentGameModeX = RaiseRightAlpha;
    }
}

qreal GameController::clamp(qreal n){
    n = n>1.0 ? 1.0:n;
    n = n<-1.0 ? -1.0:n;
    return n;
}
