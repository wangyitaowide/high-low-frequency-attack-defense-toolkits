#include "readthread.h"

ReadThread::ReadThread(QObject *parent) :
    QThread(parent)
{
    //Reader = "";
    serial = NULL;
}

void ReadThread::PrintAllInfo(Connecter *card)
{
    emit PrintCardNum(card->card_num);
    emit PrintCardType(card->card_type);
    if(!card->card_name.isEmpty())
        emit PrintCardName(card->card_name);
    if(!card->guy_name.isEmpty())
        emit PrintGuyName(card->guy_name);
    if(!card->guy_name_ex.isEmpty())
        emit PrintGuyNameEx(card->guy_name_ex);
    if(!card->id_num.isEmpty() && !card->id_type.isEmpty())
        emit PrintIdType(card->id_type);
    if(!card->id_num.isEmpty())
        emit PrintIdNum(card->id_num);
    emit PrintValidity(card->begin_time, card->end_time);

    std::list<CUSTUMRECORD>::iterator it;
    it = card->RecordList.begin();
    QStringList title;
    title << QString::fromLocal8Bit("交易时间：")
          << QString::fromLocal8Bit("交易金额：")
          << QString::fromLocal8Bit("交易地点：")
          << QString::fromLocal8Bit("交易类型：");
    emit AddInfoList(title);
    while (it != card->RecordList.end()) {
        QStringList temp;
        temp << it->Time
             << it->Money
             << it->Position
             << it->Type;
        emit AddInfoList(temp);
        it = card->RecordList.erase(it);
    }

}


void ReadThread::run()
{
    // new connect class
    // clear info struct
    // fill reader name
    // connect function
    // success
    //    emit empty string signal to GUI
    //    read
    //    emit string signal to GUI
    //    delete class
    // fail
    //    delete class
    //    return

    if (NULL == serial)
        return;

    Connecter *card = new Connecter;
    //card->ReaderName = Reader;
    //card->SetHSC(hSC);
    if(card->TestConnect() == SCARD_S_SUCCESS) {
        // send Clear info text signal;
        // blablabla
        emit ShowStatus(QString::fromLocal8Bit
                        ("已检测到银行卡，正在读取！请不要移开银行卡..."),
                        STATUS_LEVEL_WARNING);
        QTime time;
        time.start();
        card->StartReadInfo();
        qDebug()<<"Reader Time: " << time.elapsed()/1000.0 << "s";

        emit ClearText();
        // send Clear info text signal;
        // blablabla
        time.start();
        PrintAllInfo(card);
        qDebug()<<"Printer Time: " << time.elapsed()/1000.0 << "s";
        emit ShowStatus(QString::fromLocal8Bit
                        ("读取完毕！若需读取新银行卡信息可直接更换银行卡，若需重复读取本次银行卡保持银行卡不动即可！"),
                        STATUS_LEVEL_NORMAL);
        Sleep(2000);
    }


    // send all info into info text signal;
    // blablabla
    delete card;
    return;
}
