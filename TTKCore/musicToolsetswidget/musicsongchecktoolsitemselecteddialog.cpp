#include "musicsongchecktoolsitemselecteddialog.h"
#include "ui_musicsongchecktoolsitemselecteddialog.h"
#include "musicitemdelegate.h"
#include "musicuiobject.h"

MusicSongCheckToolsItemSelectedTableWidget::MusicSongCheckToolsItemSelectedTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setColumnCount(2);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 290);
}

QString MusicSongCheckToolsItemSelectedTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsItemSelectedTableWidget::createAllItems(MusicSongItems *items)
{
    setRowCount(items->count());
    for(int i=0; i<items->count(); ++i)
    {
        const MusicSongItem song = (*items)[i];
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(MUSIC_CHECK_ROLE, false);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip(song.m_itemName);
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, 280));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);
    }
}

void MusicSongCheckToolsItemSelectedTableWidget::selectedAllItems(bool check)
{
    for(int i=0; i<rowCount(); ++i)
    {
        item(i, 0)->setData(MUSIC_CHECK_ROLE, check);
    }

    if(!check)
    {
        clearSelection();
        setCurrentIndex(QModelIndex());
    }
    else
    {
        selectAll();
    }
}



MusicSongCheckToolsItemSelectedDialog::MusicSongCheckToolsItemSelectedDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSongCheckToolsItemSelectedDialog)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->selectAllCheckButton->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(accept()));
    connect(m_ui->selectAllCheckButton, SIGNAL(clicked()), SLOT(accept()));
}

MusicSongCheckToolsItemSelectedDialog::~MusicSongCheckToolsItemSelectedDialog()
{
    delete m_ui;
}

QString MusicSongCheckToolsItemSelectedDialog::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsItemSelectedDialog::createAllItems(MusicSongItems *items)
{
    m_ui->itemTableWidget->createAllItems(items);
}

int MusicSongCheckToolsItemSelectedDialog::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
