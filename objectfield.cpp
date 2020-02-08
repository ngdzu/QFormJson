#include "objectfield.h"


ObjectField::ObjectField(const QJsonObject &schema, QWidget *parent)
    :AbstractField(parent)
{
    init(schema);
}

ObjectField::ObjectField(const QByteArray &json, QWidget *parent)
    :AbstractField(parent)
{
    QJsonParseError err;
    QJsonObject obj = QJsonDocument::fromJson(json,&err).object();

    if (err.error == QJsonParseError::NoError)
        init(obj);


}

void ObjectField::parseSchema(const QJsonObject &schema)
{
    if (schema.value("type") == QJsonValue::Undefined )
        return;

    AbstractField * field = Q_NULLPTR;

    QFormLayout * propLayout = new QFormLayout;

    // We know that it is object type,
    if (schema.contains("properties"))
    {
        QJsonValue propValue= schema.value("properties");
        if(propValue.isArray())
            parseSchemaPropertyArray(propValue.toArray());
        else
            parseSchemaProperty(propValue.toObject());
    }





    if ( schema.contains("properties"))
    {
        QJsonValue value = schema.value("properties");
        if(value.isObject())
        {
            QJsonObject properties = schema.value("properties").toObject();
            parseSchemaProperty(value.toObject());
        }
        else if (value.isArray())
        {
            QJsonArray array = value.toArray();
            for (const auto &item : array)
            {
                parseSchemaProperty(item.toObject());
            }
        }
    }



    QVBoxLayout * formLayout = new QVBoxLayout;
    formLayout->addLayout(mGroupBox);
    formLayout->addStretch();
    setLayout(formLayout);


}

// Parse object
void ObjectField::parseSchemaProperty(const QJsonObject &property)
{
    QFormLayout * propLayout = new QFormLayout;

    if(property.isObject())
    {
        QJsonObject properties = schema.value("properties").toObject();
        parseSchemaProperty(value.toObject());
    }
    else if (property.isArray())
    {
        QJsonArray array = value.toArray();
        for (const auto &item : array)
        {
            parseSchemaProperty(item.toObject());
        }



//    for(QString key : properties.keys())
//    {
//        qDebug()<<key;

//        QJsonObject property = properties.value("properties").toObject();
        AbstractField * field = Q_NULLPTR;

        if (property.value("type") == "string"){
            field = new StringField(property,this);
            propLayout->addRow(field->title(),field);
        }

        if (property.value("type") == "integer"){
            field = new IntegerField(property,this);
            propLayout->addRow(field->title(),field);
        }

        if (property.value("type") == "object"){
            field                   = new ObjectField(property,this);
            QGroupBox * box         = new QGroupBox;

            QVBoxLayout * boxLayout = new QVBoxLayout;
            boxLayout->setContentsMargins(0,0,0,0);
            boxLayout->addWidget(field);
            box->setLayout(boxLayout);
            box->setTitle(field->title());
            propLayout->addWidget(box);
        }
        //    }
    }
}

void ObjectField::parseSchemaPropertyArray(const QJsonArray &array)
{
    for (const auto &item : array)
    {
        parseSchemaProperty(item.toObject());
    }
}

