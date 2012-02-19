#include "controlpanelview.h"

ControlPanelView::ControlPanelView(QWidget *parent) :
    QWidget(parent)
{
    buildView();
}

void ControlPanelView::buildView()
{
    createItems();
    arrangeItems();
    connectItems();
}

void ControlPanelView::createItems()
{
    m_clear = new QPushButton("Clear Text Fields", this);
    m_human1 = new QPushButton("Set Player 1\n to Human", this);
    m_human2 = new QPushButton("Set Player 2\n to Human", this);
    m_AI1 = new QPushButton("Set Player 1\n to AI", this);
    m_AI2 = new QPushButton("Set Player 2\n to AI", this);
    m_nextMove  = new QPushButton("Make Move", this);
    m_newGameButton = new QPushButton("Reset Game", this);
    m_createAI1 = new QPushButton("Create AI 1\nfrom this file", this);
    m_createAI2 = new QPushButton("Create AI 2\nfrom this file", this);
    m_trainAI1Button = new QPushButton("Train Neural Net Player 1", this);
    m_trainAI2Button = new QPushButton("Train Neural Net Player 2", this);

    m_AIFile1 = new QLineEdit(this);
    m_AIFile2 = new QLineEdit(this);

    m_centralLayout = new QGridLayout(this);

    setLayout(m_centralLayout);
}

void ControlPanelView::arrangeItems()
{
    m_centralLayout->addWidget(m_human1, 0, 0);
    m_centralLayout->addWidget(m_human2, 0, 1);
    m_centralLayout->addWidget(m_AI1, 0, 2);
    m_centralLayout->addWidget(m_AI2, 0, 3);

    m_centralLayout->addWidget(m_AIFile1, 2, 0, 1, 4);
    m_centralLayout->addWidget(m_createAI1, 3, 0, 1, 2);
    m_centralLayout->addWidget(m_trainAI1Button, 3, 2, 1, 2);
    m_centralLayout->addWidget(m_AIFile2, 4, 0, 1, 4);
    m_centralLayout->addWidget(m_createAI2, 5, 0, 1, 2);
    m_centralLayout->addWidget(m_trainAI2Button, 5, 2, 1, 2);

    m_centralLayout->addWidget(m_clear, 6, 1, 1, 2);
    m_centralLayout->addWidget(m_nextMove, 8, 0, 1, 2);
    m_centralLayout->addWidget(m_newGameButton, 8, 2, 1, 2);
}

void ControlPanelView::connectItems()
{
    ////
    //Connect the button clicks to their corresponding slots.
    ////

    //Clear
    connect(m_clear, SIGNAL(clicked()), this, SLOT(clearTextFields()));

    //Set human players
    connect(m_human1, SIGNAL(clicked()), this, SLOT(onSetHuman1()));
    connect(m_human2, SIGNAL(clicked()), this, SLOT(onSetHuman2()));

    //Create AI players
    connect(m_createAI1, SIGNAL(clicked()), this, SLOT(onCreateAI1()));
    connect(m_createAI2, SIGNAL(clicked()), this, SLOT(onCreateAI2()));

    //Set AI players
    connect(m_AI1, SIGNAL(clicked()), this, SLOT(onSetAI1()));
    connect(m_AI2, SIGNAL(clicked()), this, SLOT(onSetAI2()));

    //Train AI players
    connect(m_trainAI1Button, SIGNAL(clicked()), this, SLOT(onTrainAI1()));
    connect(m_trainAI2Button, SIGNAL(clicked()), this, SLOT(onTrainAI2()));

    //New game
    connect(m_newGameButton, SIGNAL(clicked()), this, SLOT(onNewGame()));

    //Next move
    connect(m_nextMove, SIGNAL(clicked()), this, SLOT(onNextMove()));
}

void ControlPanelView::clearTextFields()
{
    m_AIFile1->setText("");
    m_AIFile2->setText("");
}
