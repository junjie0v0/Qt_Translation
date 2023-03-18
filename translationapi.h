#ifndef TRANSLATIONAPI_H
#define TRANSLATIONAPI_H

#include <QWidget>

class TranslationApi : public QWidget
{
    Q_OBJECT
public:
//    explicit TranslationApi(QWidget *parent = nullptr);
    TranslationApi(QString translationApi);

    QString originalText;
    QString translationText;

signals:

};

#endif // TRANSLATIONAPI_H
