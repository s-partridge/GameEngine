#ifndef CONTROLPANELVIEW_H
#define CONTROLPANELVIEW_H

#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>

#include <QPushButton>
#include <QSpacerItem>

#include "datatypes.h"

class ControlPanelView : public QWidget
{
    Q_OBJECT
public:
    explicit ControlPanelView(QWidget *parent = 0);

signals:
    void setHuman(Elements::PlayerType player);
    void setAI(Elements::PlayerType player);

    void createAI(Elements::PlayerType player, QString filename);

    void newGame();
    void nextMove();

    void trainAI(Elements::PlayerType player);

private slots:
    //Erases the text strings in AIFile1 and AIFile2.
    void clearTextFields();

    void onCreateAI1() { emit createAI(Elements::PLAYER_1, m_AIFile1->text()); }
    void onCreateAI2() { emit createAI(Elements::PLAYER_2, m_AIFile2->text()); }

    void onTrainAI1() { emit trainAI(Elements::PLAYER_1); }
    void onTrainAI2() { emit trainAI(Elements::PLAYER_2); }

    void onSetAI1() { emit setAI(Elements::PLAYER_1); }
    void onSetAI2() { emit setAI(Elements::PLAYER_2); }

    void onSetHuman1() { emit setHuman(Elements::PLAYER_1); }
    void onSetHuman2() { emit setHuman(Elements::PLAYER_2); }

    void onNewGame() { emit newGame(); }
    void onNextMove() { emit nextMove(); }

protected:
    QPushButton *m_clear;
    QPushButton *m_human1, *m_human2;
    QPushButton *m_AI1, *m_AI2;

    QPushButton *m_createAI1, *m_createAI2;
    QPushButton *m_trainAI1Button, *m_trainAI2Button;

    QPushButton *m_nextMove;
    QPushButton *m_newGameButton;

    QLineEdit *m_AIFile1;
    QLineEdit *m_AIFile2;
    QGridLayout *m_centralLayout;

private:
    //Performs everything necessary to initialize a controlPanelView object.
    void buildView();

    //Calls the constructors for each view item.
    void createItems();
    //Arranges all view items in the central layout.
    void arrangeItems();
    //Builds the connections between
    void connectItems();
};

#endif // CONTROLPANELVIEW_H
