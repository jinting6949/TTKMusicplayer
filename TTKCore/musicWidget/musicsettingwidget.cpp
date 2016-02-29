#include "musicsettingwidget.h"
#include "ui_musicsettingwidget.h"
#include "musicbgthememanager.h"
#include "musicnetworkthread.h"
#include "musicutils.h"

#include <QFontDatabase>
#include <QColorDialog>
#include <QFileDialog>
#include <QButtonGroup>
#include <QStyledItemDelegate>

MusicFunctionTableWidget::MusicFunctionTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 20);
    headerview->resizeSection(1, 20);
    headerview->resizeSection(2, 85);

    setRowCount(3);
    m_listIndex = 0;
}

MusicFunctionTableWidget::~MusicFunctionTableWidget()
{

}

void MusicFunctionTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(column);
    emit currentIndexChanged(row + m_listIndex);
}

void MusicFunctionTableWidget::leaveEvent(QEvent *event)
{
    QTableWidget::leaveEvent(event);
    listCellEntered(-1, -1);
}

void MusicFunctionTableWidget::addFunctionItems(int index, const QStringList &icon, const QStringList &path)
{
    m_listIndex = index;
    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *item = nullptr;
        setItem(i, 0, item = new QTableWidgetItem());

                      item = new QTableWidgetItem(QIcon(icon[i]), QString());
        item->setTextAlignment(Qt::AlignCenter);
        setItem(i, 1, item);

                      item = new QTableWidgetItem(path[i]);
        item->setTextColor(QColor(80, 80, 80));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 2, item);
    }
}


MusicSettingWidget::MusicSettingWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicSettingWidget)
{
    ui->setupUi(this);

    ////////////////////////////////////////////////
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ////////////////////////////////////////////////
    ui->normalFunTableWidget->setRowCount(3);
    ui->normalFunTableWidget->addFunctionItems(0,
        QStringList() << ":/contextMenu/setting" << ":/contextMenu/funckeyboard" << ":/contextMenu/funcdownload",
        QStringList() << tr("Normal") << tr("Hotkey") << tr("Dwonload"));
    ui->lrcFunTableWidget->setRowCount(2);
    ui->lrcFunTableWidget->addFunctionItems(ui->normalFunTableWidget->rowCount(),
        QStringList() << ":/contextMenu/lrc" << ":/contextMenu/funcdesktopLrc",
        QStringList() << tr("Desktop") << tr("Inline"));
    ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(ui->normalFunTableWidget, SIGNAL(currentIndexChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui->normalFunTableWidget, SIGNAL(currentIndexChanged(int)), ui->lrcFunTableWidget, SLOT(clearSelection()));
    connect(ui->lrcFunTableWidget, SIGNAL(currentIndexChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui->lrcFunTableWidget, SIGNAL(currentIndexChanged(int)), ui->normalFunTableWidget, SLOT(clearSelection()));
    connect(ui->confirmButton, SIGNAL(clicked()), SLOT(commitTheResults()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(close()));

    ////////////////////////////////////////////////
    ui->autoPlayCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->backPlayCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->minimumRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->quitRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->setDefaultPlayerCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->closeNetWorkCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    ui->languageComboBox->setItemDelegate(new QStyledItemDelegate(ui->languageComboBox));
    ui->languageComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->languageComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->languageComboBox->addItems(QStringList() << tr("0") << tr("1") << tr("2"));
    ////////////////////////////////////////////////
    ui->showInlineCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->showDesktopCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    ////////////////////////////////////////////////
    initInlineLrcWidget();
    initDesktopLrcWidget();
    initDownloadWidget();
    ////////////////////////////////////////////////
}

MusicSettingWidget::~MusicSettingWidget()
{
    delete ui;
}

void MusicSettingWidget::initInlineLrcWidget()
{
    ui->fontComboBox->setItemDelegate(new QStyledItemDelegate(ui->fontComboBox));
    ui->fontComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->fontComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->fontSizeComboBox->setItemDelegate(new QStyledItemDelegate(ui->fontSizeComboBox));
    ui->fontSizeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->fontSizeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->fontTypeComboBox->setItemDelegate(new QStyledItemDelegate(ui->fontTypeComboBox));
    ui->fontTypeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->fontTypeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->fontDefaultColorComboBox->setItemDelegate(new QStyledItemDelegate(ui->fontDefaultColorComboBox));
    ui->fontDefaultColorComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->fontDefaultColorComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->fontComboBox->addItems(QFontDatabase().families(QFontDatabase::Any));
    ui->fontSizeComboBox->addItems(QStringList() << tr("smaller") << tr("small")
                                     << tr("middle") << tr("big") << tr("bigger"));
    ui->fontTypeComboBox->addItems(QStringList() << "1" << "2" << "3" << "4");
    ui->fontDefaultColorComboBox->addItems(QStringList() << tr("origin") << tr("red") << tr("orange")
                                                << tr("yellow") << tr("green") << tr("blue") << tr("indigo")
                                                << tr("purple") << tr("white") << tr("black"));
    connect(ui->fontComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInlineLrcDemo()));
    connect(ui->fontSizeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInlineLrcDemo()));
    connect(ui->fontTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showInlineLrcDemo()));
    connect(ui->fontDefaultColorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(defaultLrcColorChanged(int)));

    ui->transparentSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->noPlayedPushButton->setIcon(QIcon(":/color/purple"));
    ui->noPlayedPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->noPlayedPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->playedPushButton->setIcon(QIcon(":/color/purple"));
    ui->playedPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->playedPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->noPlayedPushButton, SIGNAL(clicked()), SLOT(inlineLrcBgChanged()));
    connect(ui->playedPushButton, SIGNAL(clicked()), SLOT(inlineLrcFgChanged()));
    connect(ui->transparentSlider, SIGNAL(valueChanged(int)), SLOT(inlineLrcTransChanged(int)));

    ui->resetPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->resetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->resetPushButton, SIGNAL(clicked()), SLOT(resetInlineParameter()));

    resetInlineParameter();
}

void MusicSettingWidget::initDesktopLrcWidget()
{
    ui->DfontComboBox->setItemDelegate(new QStyledItemDelegate(ui->DfontComboBox));
    ui->DfontComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->DfontComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->DfontSizeComboBox->setItemDelegate(new QStyledItemDelegate(ui->DfontSizeComboBox));
    ui->DfontSizeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->DfontSizeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->DfontTypeComboBox->setItemDelegate(new QStyledItemDelegate(ui->DfontTypeComboBox));
    ui->DfontTypeComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->DfontTypeComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->DfontDefaultColorComboBox->setItemDelegate(new QStyledItemDelegate(ui->DfontDefaultColorComboBox));
    ui->DfontDefaultColorComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->DfontDefaultColorComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->DfontComboBox->addItems(QFontDatabase().families(QFontDatabase::Any));
    ui->DfontSizeComboBox->addItems(QStringList() << tr("smaller3") << tr("smaller2") << tr("smaller") << tr("small3")
                                   << tr("small2") << tr("small") << tr("middle") << tr("big") << tr("big2") << tr("big3")
                                   << tr("bigger") << tr("bigger2") << tr("bigger3"));
    ui->DfontTypeComboBox->addItems(QStringList() << "1" << "2" << "3" << "4");
    ui->DfontDefaultColorComboBox->addItems(QStringList() << tr("origin") << tr("red") << tr("orange")
                                   << tr("yellow") << tr("green") << tr("blue") << tr("indigo") << tr("purple")
                                   << tr("white") << tr("black"));
    connect(ui->DfontComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo(int)));
    connect(ui->DfontSizeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo(int)));
    connect(ui->DfontTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(showDesktopLrcDemo(int)));
    connect(ui->DfontDefaultColorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(defaultDesktopLrcColorChanged(int)));

    ui->DtransparentSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->DnoPlayedPushButton->setIcon(QIcon(":/color/purple"));
    ui->DnoPlayedPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->DnoPlayedPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->DplayedPushButton->setIcon(QIcon(":/color/purple"));
    ui->DplayedPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->DplayedPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->DnoPlayedPushButton, SIGNAL(clicked()), SLOT(desktopBgChanged()));
    connect(ui->DplayedPushButton, SIGNAL(clicked()), SLOT(desktopFgChanged()));
    connect(ui->DtransparentSlider, SIGNAL(valueChanged(int)), SLOT(desktopLrcTransChanged(int)));

    ui->DresetPushButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->DresetPushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->DresetPushButton, SIGNAL(clicked()), SLOT(resetDesktopParameter()));

    resetDesktopParameter();
}

void MusicSettingWidget::initDownloadWidget()
{
    ui->downloadDirButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->downloadLrcDirButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->downloadDirButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->downloadLrcDirButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->downloadCacheAutoRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->downloadCacheManRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->downloadFullRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->downloadLimitRadioBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);

    ui->downloadServerComboBox->setItemDelegate(new QStyledItemDelegate(ui->downloadServerComboBox));
    ui->downloadServerComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->downloadServerComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->downloadLimitSpeedComboBox->setItemDelegate(new QStyledItemDelegate(ui->downloadLimitSpeedComboBox));
    ui->downloadLimitSpeedComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->downloadLimitSpeedComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->uploadLimitSpeedComboBox->setItemDelegate(new QStyledItemDelegate(ui->uploadLimitSpeedComboBox));
    ui->uploadLimitSpeedComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->uploadLimitSpeedComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    ui->downloadSpinBox->setRange(1024, 5*1024);
    ui->downloadDirEdit->setText(MUSIC_DOWNLOAD_AL);
    ui->downloadLrcDirEdit->setText(LRC_DOWNLOAD_AL);
    QStringList downloadSpeed;
    downloadSpeed << "100" << "200" << "300" << "400" << "500" << "600"
                  << "700" << "800" << "900" << "1000" << "1100" << "1200";
    ui->downloadLimitSpeedComboBox->addItems(downloadSpeed);
    ui->uploadLimitSpeedComboBox->addItems(downloadSpeed);

    ui->downloadServerComboBox->addItem(QIcon(":/server/wangyiyun"), tr("wangyiMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/aiyinyue"), tr("dianxinMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/qq"), tr("qqMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/xiami"), tr("xiamiMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/ttdongting"), tr("ttpodMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/baidu"), tr("baiduMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/kuwo"), tr("kuwoMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/kugou"), tr("kuhouMusic"));
    ui->downloadServerComboBox->addItem(QIcon(":/server/duomi"), tr("duomiMusic"));

    /////////////////////////////////////////////////////////////
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->downloadCacheAutoRadioBox, 0);
    buttonGroup->addButton(ui->downloadCacheManRadioBox, 1);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(downloadGroupCached(int)));

    QButtonGroup *buttonGroup2 = new QButtonGroup(this);
    buttonGroup2->addButton(ui->downloadFullRadioBox, 0);
    buttonGroup2->addButton(ui->downloadLimitRadioBox, 1);
    connect(buttonGroup2, SIGNAL(buttonClicked(int)), SLOT(downloadGroupSpeedLimit(int)));

    QButtonGroup *buttonGroup3 = new QButtonGroup(this);
    buttonGroup3->addButton(ui->downloadDirButton, 0);
    buttonGroup3->addButton(ui->downloadLrcDirButton, 1);
    connect(buttonGroup3, SIGNAL(buttonClicked(int)), SLOT(downloadDirSelected(int)));

    ui->downloadCacheAutoRadioBox->click();
    ui->downloadFullRadioBox->click();
}

void MusicSettingWidget::initControllerParameter()
{
    //Set init parameter
    ui->autoPlayCheckBox->setChecked(M_SETTING->value(MusicSettingManager::AutoPlayChoiced).toBool());
    ui->backPlayCheckBox->setChecked(M_SETTING->value(MusicSettingManager::LastPlayIndexChoiced)
               .toStringList().first().toInt() );
    if(!M_SETTING->value(MusicSettingManager::CloseEventChoiced).toBool())
    {
        ui->minimumRadioBox->setChecked(true);
    }
    else
    {
        ui->quitRadioBox->setChecked(true);
    }
    ui->languageComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::CurrentLanIndexChoiced).toInt());

    ////////////////////////////////////////////////
    //Set init parameter
    ui->showInlineCheckBox->setChecked(M_SETTING->value(MusicSettingManager::ShowInlineLrcChoiced).toBool());
    ui->showDesktopCheckBox->setChecked(M_SETTING->value(MusicSettingManager::ShowDesktopLrcChoiced).toBool());
    ui->showInlineCheckBox->setEnabled(false);

    ui->fontComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::LrcFamilyChoiced).toInt());
    ui->fontSizeComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::LrcSizeChoiced).toInt() - 13);
    ui->fontTypeComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::LrcTypeChoiced).toInt());
    ui->fontDefaultColorComboBox->setCurrentIndex(-1);
    if(M_SETTING->value(MusicSettingManager::LrcColorChoiced).toInt() != -1)
    {
        ui->fontDefaultColorComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::LrcColorChoiced).toInt());
    }
    else
    {
        QPixmap pixmap(16, 16);
        pixmap.fill(m_lrcSelectedFg = M_SETTING->value(MusicSettingManager::LrcFgColorChoiced).value<QColor>());
        ui->playedPushButton->setIcon(QIcon(pixmap));
        pixmap.fill(m_lrcSelectedBg = M_SETTING->value(MusicSettingManager::LrcBgColorChoiced).value<QColor>());
        ui->noPlayedPushButton->setIcon(QIcon(pixmap));
        ui->showLabel->setLinearGradient(m_lrcSelectedFg, m_lrcSelectedBg);
        ui->showLabel->update();
    }
    ui->transparentSlider->setValue(M_SETTING->value(MusicSettingManager::LrcColorTransChoiced).toInt());

    ////////////////////////////////////////////////
    ui->DfontComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::DLrcFamilyChoiced).toInt());
    ui->DfontSizeComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::DLrcSizeChoiced).toInt() - 24);
    ui->DfontTypeComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::DLrcTypeChoiced).toInt());
    ui->DfontDefaultColorComboBox->setCurrentIndex(-1);
    if(M_SETTING->value(MusicSettingManager::DLrcColorChoiced).toInt() != -1)
    {
        ui->DfontDefaultColorComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::DLrcColorChoiced).toInt());
    }
    else
    {
        QPixmap pixmap(16, 16);
        pixmap.fill(m_DlrcSelectedFg = M_SETTING->value(MusicSettingManager::DLrcFgColorChoiced).value<QColor>());
        ui->DplayedPushButton->setIcon(QIcon(pixmap));
        pixmap.fill(m_DlrcSelectedBg = M_SETTING->value(MusicSettingManager::DLrcBgColorChoiced).value<QColor>());
        ui->DnoPlayedPushButton->setIcon(QIcon(pixmap));
        ui->DshowLabel->setLinearGradient(m_DlrcSelectedFg, m_DlrcSelectedBg);
        ui->DshowLabel->update();
    }
    ui->DtransparentSlider->setValue(M_SETTING->value(MusicSettingManager::DLrcColorTransChoiced).toInt());

    ////////////////////////////////////////////////
    ui->downloadDirEdit->setText(M_SETTING->value(MusicSettingManager::DownloadMusicPathDirChoiced).toString());
    ui->downloadLrcDirEdit->setText(M_SETTING->value(MusicSettingManager::DownloadLrcPathDirChoiced).toString());
    ui->downloadSpinBox->setValue(M_SETTING->value(MusicSettingManager::DownloadCacheSizeChoiced).toInt());
    M_SETTING->value(MusicSettingManager::DownloadCacheLimitChoiced).toInt() == 1 ?
                     ui->downloadCacheAutoRadioBox->click() : ui->downloadCacheManRadioBox->click();
#ifdef MUSIC_QT_5
    ui->downloadLimitSpeedComboBox->setCurrentText(M_SETTING->value(MusicSettingManager::DownloadDLoadLimitChoiced).toString());
    ui->uploadLimitSpeedComboBox->setCurrentText(M_SETTING->value(MusicSettingManager::DownloadULoadLimitChoiced).toString());
#else
    MusicUtils::setComboboxText(ui->downloadLimitSpeedComboBox, M_SETTING->value(MusicSettingManager::DownloadDLoadLimitChoiced).toString());
    MusicUtils::setComboboxText(ui->uploadLimitSpeedComboBox, M_SETTING->value(MusicSettingManager::DownloadULoadLimitChoiced).toString());
#endif
    M_SETTING->value(MusicSettingManager::DownloadLimitChoiced).toInt() == 1 ?
                     ui->downloadFullRadioBox->click() : ui->downloadLimitRadioBox->click();
    ///////////////////////////////////////////////////////////////////////////

    ui->downloadServerComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::DownloadServerChoiced).toInt());
    ui->closeNetWorkCheckBox->setChecked(M_SETTING->value(MusicSettingManager::CloseNetWorkChoiced).toInt());
    ui->setDefaultPlayerCheckBox->setChecked(M_SETTING->value(MusicSettingManager::FileAssociationChoiced).toInt());
    if(ui->setDefaultPlayerCheckBox->isChecked())
    {
        ui->setDefaultPlayerCheckBox->setEnabled(false);
    }
}

void MusicSettingWidget::changeInlineLrcWidget()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MusicSettingWidget::changeDesktopLrcWidget()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MusicSettingWidget::commitTheResults()
{
    M_SETTING->setValue(MusicSettingManager::CurrentLanIndexChoiced, ui->languageComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::AutoPlayChoiced, ui->autoPlayCheckBox->isChecked());
    QStringList list = M_SETTING->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    list[0] = QString::number(ui->backPlayCheckBox->isChecked());
    M_SETTING->setValue(MusicSettingManager::LastPlayIndexChoiced, list);
    M_SETTING->setValue(MusicSettingManager::CloseEventChoiced, ui->quitRadioBox->isChecked() ? "true" : "false");

    M_SETTING->setValue(MusicSettingManager::ShowInlineLrcChoiced, ui->showInlineCheckBox->isChecked());
    M_SETTING->setValue(MusicSettingManager::ShowDesktopLrcChoiced, ui->showDesktopCheckBox->isChecked());

    M_SETTING->setValue(MusicSettingManager::LrcColorChoiced, ui->fontDefaultColorComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::LrcFamilyChoiced, ui->fontComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::LrcSizeChoiced, ui->fontSizeComboBox->currentIndex() + 13);
    M_SETTING->setValue(MusicSettingManager::LrcTypeChoiced, ui->fontTypeComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::LrcColorTransChoiced, ui->transparentSlider->value());

    M_SETTING->setValue(MusicSettingManager::DLrcColorChoiced, ui->DfontDefaultColorComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::DLrcFamilyChoiced, ui->DfontComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::DLrcSizeChoiced, ui->DfontSizeComboBox->currentIndex() + 24);
    M_SETTING->setValue(MusicSettingManager::DLrcTypeChoiced, ui->DfontTypeComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::DLrcColorTransChoiced, ui->DtransparentSlider->value());

    M_SETTING->setValue(MusicSettingManager::DLrcFgColorChoiced, m_DlrcSelectedFg);
    M_SETTING->setValue(MusicSettingManager::DLrcBgColorChoiced, m_DlrcSelectedBg);
    M_SETTING->setValue(MusicSettingManager::LrcFgColorChoiced, m_lrcSelectedFg);
    M_SETTING->setValue(MusicSettingManager::LrcBgColorChoiced, m_lrcSelectedBg);

    M_SETTING->setValue(MusicSettingManager::DownloadMusicPathDirChoiced, ui->downloadDirEdit->text());
    M_SETTING->setValue(MusicSettingManager::DownloadLrcPathDirChoiced, ui->downloadLrcDirEdit->text());
    M_SETTING->setValue(MusicSettingManager::DownloadCacheLimitChoiced, ui->downloadCacheAutoRadioBox->isChecked());
    M_SETTING->setValue(MusicSettingManager::DownloadCacheSizeChoiced, ui->downloadSpinBox->value());
    M_SETTING->setValue(MusicSettingManager::DownloadLimitChoiced, ui->downloadFullRadioBox->isChecked());
    M_SETTING->setValue(MusicSettingManager::DownloadServerChoiced, ui->downloadServerComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::DownloadDLoadLimitChoiced, ui->downloadLimitSpeedComboBox->currentText());
    M_SETTING->setValue(MusicSettingManager::DownloadULoadLimitChoiced, ui->uploadLimitSpeedComboBox->currentText());

    M_NETWORK->setBlockNetWork( ui->closeNetWorkCheckBox->isChecked() );
    M_SETTING->setValue(MusicSettingManager::FileAssociationChoiced, ui->setDefaultPlayerCheckBox->isChecked());
    if(ui->setDefaultPlayerCheckBox->isChecked())
    {
        ui->setDefaultPlayerCheckBox->setEnabled(false);
    }

    emit parameterSettingChanged();
    close();
}

void MusicSettingWidget::inlineLrcFgChanged()
{
    lcrColorValue(Inline, "LRCFGCOLORCHOICED", ui->playedPushButton);
}

void MusicSettingWidget::desktopFgChanged()
{
    lcrColorValue(Desktop, "DLRCFGCOLORCHOICED", ui->DplayedPushButton);
}

void MusicSettingWidget::inlineLrcBgChanged()
{
    lcrColorValue(Inline, "LRCBGCOLORCHOICED", ui->noPlayedPushButton);
}

void MusicSettingWidget::desktopBgChanged()
{
    lcrColorValue(Desktop, "DLRCBGCOLORCHOICED", ui->DnoPlayedPushButton);
}

void MusicSettingWidget::lcrColorValue(Type key, const QString &type, QPushButton *obj)
{
    key == Inline ? ui->fontDefaultColorComboBox->setCurrentIndex(-1)
                  : ui->DfontDefaultColorComboBox->setCurrentIndex(-1);
    QColorDialog getColor(Qt::white, this);
    if(getColor.exec())
    {
        QColor color = getColor.selectedColor();
        QPixmap pixmap(16, 16);
        pixmap.fill(color);
        obj->setIcon(QIcon(pixmap));
        if(type == "DLRCFGCOLORCHOICED") m_DlrcSelectedFg = color;
        if(type == "DLRCBGCOLORCHOICED") m_DlrcSelectedBg = color;
        if(type == "LRCFGCOLORCHOICED") m_lrcSelectedFg = color;
        if(type == "LRCBGCOLORCHOICED") m_lrcSelectedBg = color;
    }
    key == Inline ? showInlineLrcDemo() : showDesktopLrcDemo();
}

void MusicSettingWidget::defaultDesktopLrcColorChanged(int index)
{
    lrcColorByDefault(Desktop, index);
}

void MusicSettingWidget::defaultLrcColorChanged(int index)
{
    lrcColorByDefault(Inline, index);
}

void MusicSettingWidget::lrcColorByDefault(Type key, int index)
{
    if(index == -1)
    {
        return;
    }
    QColor color(Qt::black);
    switch(index)
    {
        case 0: color = QColor(14, 179, 255);break;
        case 1: color = QColor(214, 51, 44);break;
        case 2: color = QColor(230, 130, 52);break;
        case 3: color = QColor(243, 209, 0);break;
        case 4: color = QColor(62, 164, 140);break;
        case 5: color = QColor(76, 147, 193);break;
        case 6: color = QColor(29, 237, 235);break;
        case 7: color = QColor(156, 115, 155);break;
        case 8: color = QColor(255, 255, 255);break;
        case 9: color = QColor(0, 0, 0);break;
        default: break;
    }

    QPixmap pixmap(16, 16);
    pixmap.fill(color);
    key == Inline ? ui->noPlayedPushButton->setIcon(QIcon(pixmap))
                  : ui->DnoPlayedPushButton->setIcon(QIcon(pixmap));
    pixmap.fill(QColor(222, 54, 4));
    key == Inline ? ui->playedPushButton->setIcon(QIcon(pixmap))
                  : ui->DplayedPushButton->setIcon(QIcon(pixmap));

    key == Inline ? m_lrcSelectedFg = QColor(222, 54, 4) : m_DlrcSelectedFg = QColor(222, 54, 4);
    key == Inline ? m_lrcSelectedBg = color : m_DlrcSelectedBg = color;
    key == Inline ? showInlineLrcDemo() : showDesktopLrcDemo();
}

void MusicSettingWidget::lrcTransparentValue(Type key, int value) const
{
    MusicPreviewLabel* label;
    QColor fcolor = key == Inline ? m_lrcSelectedFg : m_DlrcSelectedFg;
    QColor bcolor = key == Inline ? m_lrcSelectedBg : m_DlrcSelectedBg;
    fcolor.setAlpha(2.55*value);
    bcolor.setAlpha(2.55*value);
    key == Inline ? label = ui->showLabel : label = ui->DshowLabel;
    label->setTransparent(2.55*value);
    label->setLinearGradient(fcolor, bcolor);
    label->update();
}

void MusicSettingWidget::inlineLrcTransChanged(int index)
{
    lrcTransparentValue(Inline, index);
}

void MusicSettingWidget::desktopLrcTransChanged(int index)
{
    lrcTransparentValue(Desktop, index);
}

void MusicSettingWidget::showInlineLrcDemo()
{
    QStringList para;
    para << ui->fontComboBox->currentText()
         << QString::number(ui->fontSizeComboBox->currentIndex())
         << QString::number(ui->fontTypeComboBox->currentIndex());

    ui->showLabel->setLinearGradient(m_lrcSelectedFg,m_lrcSelectedBg);
    ui->showLabel->setParameter(para);
    ui->showLabel->update();
}

void MusicSettingWidget::showDesktopLrcDemo()
{
    QStringList para;
    para << ui->DfontComboBox->currentText()
         << QString::number(ui->DfontSizeComboBox->currentIndex())
         << QString::number(ui->DfontTypeComboBox->currentIndex());

    ui->DshowLabel->setLinearGradient(m_DlrcSelectedFg,m_DlrcSelectedBg);
    ui->DshowLabel->setParameter(para);
    ui->DshowLabel->update();
}

void MusicSettingWidget::resetInlineParameter()
{
    ui->fontComboBox->setCurrentIndex(0);
    ui->fontSizeComboBox->setCurrentIndex(0);
    ui->fontTypeComboBox->setCurrentIndex(0);
    ui->fontDefaultColorComboBox->setCurrentIndex(0);
    ui->transparentSlider->setValue(100);
}

void MusicSettingWidget::resetDesktopParameter()
{
    ui->DfontComboBox->setCurrentIndex(0);
    ui->DfontSizeComboBox->setCurrentIndex(0);
    ui->DfontTypeComboBox->setCurrentIndex(0);
    ui->DfontDefaultColorComboBox->setCurrentIndex(0);
    ui->DtransparentSlider->setValue(100);
}

int MusicSettingWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}

void MusicSettingWidget::downloadGroupCached(int index)
{
    ui->downloadSpinBox->setEnabled(index);
}

void MusicSettingWidget::downloadGroupSpeedLimit(int index)
{
    ui->downloadLimitSpeedComboBox->setEnabled(index);
    ui->uploadLimitSpeedComboBox->setEnabled(index);
}

void MusicSettingWidget::downloadDirSelected(int index)
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory );
    dialog.setViewMode(QFileDialog::Detail);
    if(dialog.exec())
    {
        QString path;
        if(!(path = dialog.directory().absolutePath()).isEmpty())
        {
            index == 0 ? ui->downloadDirEdit->setText(path)
                       : ui->downloadLrcDirEdit->setText(path);
        }
    }
}