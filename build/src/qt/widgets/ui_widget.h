/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "viewer.h"

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    Viewer *CloudViewer;
    QGraphicsView *showImageGV;
    QTabWidget *infoTab;
    QWidget *choose;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_3;
    QCheckBox *cloudCB;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QCheckBox *groundCB;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *obstacleCB;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_4;
    QCheckBox *insertCB;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_5;
    QCheckBox *lineCB;
    QWidget *tab;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_6;
    QSpinBox *paramSB;
    QLabel *label_7;
    QDoubleSpinBox *paramDSB;
    QPlainTextEdit *paramPTE;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *updatePB;
    QPushButton *clearSelectionPB;
    QPushButton *resetBT;
    QPushButton *quit;
    QHBoxLayout *horizontalLayout;
    QSpinBox *dataSeqSB;
    QPushButton *openFolderBT;
    QPushButton *playBT;
    QSpinBox *DataIdxSBox;
    QSlider *DataIdxVSlider;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(1189, 880);
        verticalLayout_2 = new QVBoxLayout(Widget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        CloudViewer = new Viewer(Widget);
        CloudViewer->setObjectName(QStringLiteral("CloudViewer"));
        CloudViewer->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CloudViewer->sizePolicy().hasHeightForWidth());
        CloudViewer->setSizePolicy(sizePolicy);
        CloudViewer->setMouseTracking(false);
        showImageGV = new QGraphicsView(CloudViewer);
        showImageGV->setObjectName(QStringLiteral("showImageGV"));
        showImageGV->setGeometry(QRect(40, 0, 827, 250));

        horizontalLayout_2->addWidget(CloudViewer);

        infoTab = new QTabWidget(Widget);
        infoTab->setObjectName(QStringLiteral("infoTab"));
        infoTab->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(infoTab->sizePolicy().hasHeightForWidth());
        infoTab->setSizePolicy(sizePolicy1);
        infoTab->setMinimumSize(QSize(280, 0));
        QFont font;
        font.setPointSize(9);
        infoTab->setFont(font);
        choose = new QWidget();
        choose->setObjectName(QStringLiteral("choose"));
        choose->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        layoutWidget = new QWidget(choose);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 271, 181));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_6->addWidget(label_3);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        cloudCB = new QCheckBox(layoutWidget);
        cloudCB->setObjectName(QStringLiteral("cloudCB"));

        horizontalLayout_6->addWidget(cloudCB);


        verticalLayout_3->addLayout(horizontalLayout_6);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_4->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        groundCB = new QCheckBox(layoutWidget);
        groundCB->setObjectName(QStringLiteral("groundCB"));

        horizontalLayout_4->addWidget(groundCB);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_5->addWidget(label_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        obstacleCB = new QCheckBox(layoutWidget);
        obstacleCB->setObjectName(QStringLiteral("obstacleCB"));

        horizontalLayout_5->addWidget(obstacleCB);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_7->addWidget(label_4);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);

        insertCB = new QCheckBox(layoutWidget);
        insertCB->setObjectName(QStringLiteral("insertCB"));

        horizontalLayout_7->addWidget(insertCB);


        verticalLayout_3->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_8->addWidget(label_5);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_5);

        lineCB = new QCheckBox(layoutWidget);
        lineCB->setObjectName(QStringLiteral("lineCB"));

        horizontalLayout_8->addWidget(lineCB);


        verticalLayout_3->addLayout(horizontalLayout_8);

        infoTab->addTab(choose, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        layoutWidget1 = new QWidget(tab);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(0, 0, 281, 781));
        verticalLayout_4 = new QVBoxLayout(layoutWidget1);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setSizeConstraint(QLayout::SetMaximumSize);
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalLayout_9->setSizeConstraint(QLayout::SetMaximumSize);
        label_6 = new QLabel(layoutWidget1);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_9->addWidget(label_6);

        paramSB = new QSpinBox(layoutWidget1);
        paramSB->setObjectName(QStringLiteral("paramSB"));

        horizontalLayout_9->addWidget(paramSB);

        label_7 = new QLabel(layoutWidget1);
        label_7->setObjectName(QStringLiteral("label_7"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy2);

        horizontalLayout_9->addWidget(label_7);

        paramDSB = new QDoubleSpinBox(layoutWidget1);
        paramDSB->setObjectName(QStringLiteral("paramDSB"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(paramDSB->sizePolicy().hasHeightForWidth());
        paramDSB->setSizePolicy(sizePolicy3);

        horizontalLayout_9->addWidget(paramDSB);


        verticalLayout_4->addLayout(horizontalLayout_9);

        paramPTE = new QPlainTextEdit(layoutWidget1);
        paramPTE->setObjectName(QStringLiteral("paramPTE"));

        verticalLayout_4->addWidget(paramPTE);

        infoTab->addTab(tab, QString());

        horizontalLayout_2->addWidget(infoTab);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        updatePB = new QPushButton(Widget);
        updatePB->setObjectName(QStringLiteral("updatePB"));

        horizontalLayout_3->addWidget(updatePB);

        clearSelectionPB = new QPushButton(Widget);
        clearSelectionPB->setObjectName(QStringLiteral("clearSelectionPB"));

        horizontalLayout_3->addWidget(clearSelectionPB);

        resetBT = new QPushButton(Widget);
        resetBT->setObjectName(QStringLiteral("resetBT"));

        horizontalLayout_3->addWidget(resetBT);

        quit = new QPushButton(Widget);
        quit->setObjectName(QStringLiteral("quit"));

        horizontalLayout_3->addWidget(quit);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        dataSeqSB = new QSpinBox(Widget);
        dataSeqSB->setObjectName(QStringLiteral("dataSeqSB"));

        horizontalLayout->addWidget(dataSeqSB);

        openFolderBT = new QPushButton(Widget);
        openFolderBT->setObjectName(QStringLiteral("openFolderBT"));

        horizontalLayout->addWidget(openFolderBT);

        playBT = new QPushButton(Widget);
        playBT->setObjectName(QStringLiteral("playBT"));

        horizontalLayout->addWidget(playBT);

        DataIdxSBox = new QSpinBox(Widget);
        DataIdxSBox->setObjectName(QStringLiteral("DataIdxSBox"));

        horizontalLayout->addWidget(DataIdxSBox);

        DataIdxVSlider = new QSlider(Widget);
        DataIdxVSlider->setObjectName(QStringLiteral("DataIdxVSlider"));
        DataIdxVSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(DataIdxVSlider);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(Widget);
        QObject::connect(quit, SIGNAL(clicked()), Widget, SLOT(close()));
        QObject::connect(DataIdxSBox, SIGNAL(valueChanged(int)), DataIdxVSlider, SLOT(setValue(int)));
        QObject::connect(DataIdxVSlider, SIGNAL(valueChanged(int)), DataIdxSBox, SLOT(setValue(int)));
        QObject::connect(quit, SIGNAL(clicked()), Widget, SLOT(close()));

        infoTab->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        label_3->setText(QApplication::translate("Widget", "               orgin   ", Q_NULLPTR));
        cloudCB->setText(QApplication::translate("Widget", "    cloud      ", Q_NULLPTR));
        label->setText(QApplication::translate("Widget", "           Ground", Q_NULLPTR));
        groundCB->setText(QApplication::translate("Widget", "  ground     ", Q_NULLPTR));
        label_2->setText(QApplication::translate("Widget", "           obstacle", Q_NULLPTR));
        obstacleCB->setText(QApplication::translate("Widget", "obstacle    ", Q_NULLPTR));
        label_4->setText(QApplication::translate("Widget", "             insert    ", Q_NULLPTR));
        insertCB->setText(QApplication::translate("Widget", "    insert     ", Q_NULLPTR));
        label_5->setText(QApplication::translate("Widget", "                line     ", Q_NULLPTR));
        lineCB->setText(QApplication::translate("Widget", "     line       ", Q_NULLPTR));
        infoTab->setTabText(infoTab->indexOf(choose), QApplication::translate("Widget", "\351\241\265", Q_NULLPTR));
        label_6->setText(QApplication::translate("Widget", "paramID", Q_NULLPTR));
        label_7->setText(QApplication::translate("Widget", "value", Q_NULLPTR));
        paramPTE->setPlainText(QApplication::translate("Widget", "groundRemove:                                           ID\n"
"line_search_angle(2)                                   0\n"
"max_slope(0.35)                                           1\n"
"tHmin(-2.15)                                                   2\n"
"tHmax(1.0)                                                      3\n"
"tHDiff(0.2)                                                       4\n"
"hSensor(1.73)                                                 5\n"
"r_min_bin(0.05)                                              6\n"
"r_max_bin(2)                                                   7\n"
"r_min_square(3 * 3)                                      8\n"
"r_max_square(120 * 120)                            9\n"
"n_bins(120)                                                     10\n"
"n_segments(240)                                          11\n"
"null                                                                    12\n"
"max_dist_to_line(0.15)                               13\n"
"null                        "
                        "                                            14\n"
"max_error_square(0.01)                             15\n"
"long_threshold(2.0)                                      16\n"
"max_long_height(0.2)                                  17\n"
"max_start_height(0.3)                                 18\n"
"sensor_height(1.70)                                     19\n"
"null                                                                     20\n"
"null                                                                     21\n"
"min_split_dist(0.1)                                         22\n"
"theta_start(65.1277)                                     23\n"
"theta_end(2)                                                    24\n"
"angle_resolution(0.41)                                 25", Q_NULLPTR));
        infoTab->setTabText(infoTab->indexOf(tab), QApplication::translate("Widget", "\351\241\265", Q_NULLPTR));
        updatePB->setText(QApplication::translate("Widget", "update", Q_NULLPTR));
        clearSelectionPB->setText(QApplication::translate("Widget", "clearSelection", Q_NULLPTR));
        resetBT->setText(QApplication::translate("Widget", "reset", Q_NULLPTR));
        quit->setText(QApplication::translate("Widget", "quit", Q_NULLPTR));
        openFolderBT->setText(QApplication::translate("Widget", "Open Folder", Q_NULLPTR));
        playBT->setText(QApplication::translate("Widget", "Play", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
