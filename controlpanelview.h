#ifndef CONTROLPANELVIEW_H
#define CONTROLPANELVIEW_H

#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
#include <QWidget>

class ControlPanelView : public QWidget
{
    Q_OBJECT
public:
    explicit ControlPanelView(QWidget *parent = 0);

signals:

public slots:

protected:
    QPushButton *human1, *human2;
    QPushButton *AI1, *AI2;
    QPushButton *newGameButton, *resetButton;
    QPushButton *trainAIButton;

    QLineEdit *AIFile1, *AIFile2;


};

#endif // CONTROLPANELVIEW_H
