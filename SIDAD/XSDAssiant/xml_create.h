#ifndef XMLCREATE_H
#define XMLCREATE_H

#include <QObject>
#include <QXmlStreamReader>
#include <QtXml/QDomDocument>
#include <QFile>
#include "xsd_type.h"
#include "type.h"
#include <QDebug>

class XmlCreate : public QObject
{
    Q_OBJECT
public:
    XmlCreate():file_name(PATH + "_tempxml_.xml"){
    }
    XmlCreate(QString filename):file_name(filename){
    }
    QString file_name;
    QDomDocument doc;

    void setfilename(QString filename);
    //节点
    XSDSystem *setSystemInfo(XSDSystem *xsdsystem);
    Node *setNodeInfo(Node *node,QDomElement &element);
    XSDSubSystem *setSubSystemInfo(XSDSubSystem *subsystem, QDomElement &element);
    XSDDevice *setDeviceInfo(XSDDevice *dev,QDomElement &element);
    XSDSwitch *setSwitchInfo(XSDSwitch *switches,QDomElement &element);
    XSDNetworkInterface *setNetworkInterfaceInfo(XSDNetworkInterface *interface,QDomElement &element);

    //软件集合
    SoftwareSet *setSoftwareSetInfo(SoftwareSet *softwareset,QDomElement &element);
    XSDSoftWare *setSoftwareInfo(XSDSoftWare *software,QDomElement &element);

    //消息集合
    MsgSet *setMsgSetInfo(MsgSet *msgset,QDomElement &element);
    Msg *setMsgInfo(Msg *msg,QDomElement &element);
    MsgStruct *setMsgStructInfo(MsgStruct *msgstruct,QDomElement &element);
    MsgElement *setMsgElementInfo(MsgElement *msgelement,QDomElement &element);

    //接口关系集合
    InterfaceRelationSet *setInterfaceRelationSetInfo(InterfaceRelationSet *set,QDomElement &element);
    SoftwareConnection *setSoftwareConnectionInfo(SoftwareConnection *connection,QDomElement &element);
    SubSoftwareConnection *setSubSoftwareConnectionInfo(SubSoftwareConnection* connection,QDomElement &element);

    InterfaceRelation *setInterfaceRelationInfo(InterfaceRelation *relation,QDomElement &element);
    DDSInterfaceRelation *setDDSInterfaceRelationInfo(DDSInterfaceRelation *relation,QDomElement &element);
    PublishMember *setPublishMemberInfo(PublishMember *publish,QDomElement &element);
    SubscribeMember *setSubscribeMemberInfo(SubscribeMember *subscribe,QDomElement &element);
    DDSConfigure *setDDSConfigureInfo(DDSConfigure *conf,QDomElement &element);
    Configure *setConfigureInfo(Configure *conf,QDomElement &element);
    QosProfile *setQosProfileInfo(QosProfile *profile,QDomElement &element);


signals:
    void sendString(QString qstr);
public slots:
};

#endif // XMLCREATE_H
