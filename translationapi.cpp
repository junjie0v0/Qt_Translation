#include "translationapi.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QDebug>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QApplication>
#include <QClipboard>

//TranslationApi::TranslationApi(QWidget *parent)
//    : QWidget{parent}
//{

//}

TranslationApi::TranslationApi(QString translationApi)
{

    //获取剪贴板的内容
    QClipboard *clipboard = QApplication::clipboard();
    originalText = clipboard->text();

    if(translationApi == "百度")
    {
        qDebug()<<originalText;
        QString appid = "20200605000486827";
        QString secretKey = "zXSrrOleTURBbSQvOcPD";
        // 计算待翻译文本的MD5值
        QString sign;
        sign = QString("%1%2%3%4").arg(appid).arg(originalText).arg("1435660288").arg(secretKey);
        QByteArray md5_sign = sign.toUtf8();
        QByteArray hash_bytes = QCryptographicHash::hash(md5_sign, QCryptographicHash::Md5); // 使用MD5算法进行加密
        QString hash_string = QString(hash_bytes.toHex()); // 将加密结果转换为QString

        // 构造请求
        QUrl url(QString("http://api.fanyi.baidu.com/api/trans/vip/translate"));
        QUrlQuery query;
        query.addQueryItem("q", originalText);
        query.addQueryItem("from", "en");
        query.addQueryItem("to", "zh");
        query.addQueryItem("appid", appid);
        query.addQueryItem("salt", "1435660288");
        query.addQueryItem("sign", hash_string);
        url.setQuery(query);

        // 发送请求并解析响应
        QNetworkAccessManager manager;
        QNetworkRequest request(url);
        QNetworkReply *reply = manager.get(request);
        while (!reply->isFinished()) {
            qApp->processEvents();
        }
        QByteArray data = reply->readAll();
        QJsonDocument json_doc = QJsonDocument::fromJson(data);

        /*json文档解析*/
        if (!json_doc.isNull())         //解析未发生错误
        {
            if (json_doc.isObject())    //解析文档为对象
            {
                QJsonObject object = json_doc.object();     //转化为对象
                if (object.contains("from"))        //包含指定的 key
                {
                    QJsonValue value = object.value("from");    //获取指定的 key值 对应的 value 值
                    if (value.isString())       //判断 value 是否为字符串
                    {
                        QString strName = value.toString();     //将 value 转化为字符串
                        qDebug()<<"from:"<<strName;
                    }
                }

                if (object.contains("to"))        //包含指定的 key
                {
                    QJsonValue value = object.value("to");    //获取指定的 key值 对应的 value 值
                    if (value.isString())       //判断 value 是否为字符串
                    {
                        QString strName = value.toString();     //将 value 转化为字符串
                        qDebug()<<"to:"<<strName;
                    }
                }

                //跟键获取值
                QJsonValue resultValue = object.value("trans_result");
                //判断类型是否为数组类型
                if (resultValue.type() == QJsonValue::Array)
                {
                    //转换为数组类型
                    QJsonArray resultArray = resultValue.toArray();
                    //遍历数组
                    for (int i = 0;i<resultArray.count();i++)
                    {
                        //获取数组的第一个元素，类型为QJsonValue
                        QJsonValue resultValueChild = resultArray.at(i);
                        //判断是不是对象类型
                        if(resultValueChild.type() == QJsonValue::Object)
                        {
                            //转化成对象类型
                            QJsonObject resultObj = resultValueChild.toObject();
                            //最后通过Value函数就可以获取到值了
                            QJsonValue dstValue = resultObj.value("dst");
                            translationText = dstValue.toString();
                            qDebug()<<"dst:"<<dstValue.toString();
                        }
                        if(resultValueChild.type() == QJsonValue::Object)
                        {
                            //转化成对象类型
                            QJsonObject resultObj = resultValueChild.toObject();
                            //最后通过Value函数就可以获取到值了
                            QJsonValue srcValue = resultObj.value("src");
                            qDebug()<<"src:"<<srcValue.toString();
                        }
                    }
                }
            }
        }

        QString jsonString = QString::fromUtf8(json_doc.toJson().constData(), json_doc.toJson().length()); //将QByteArray转换为QStirng
    }
}
