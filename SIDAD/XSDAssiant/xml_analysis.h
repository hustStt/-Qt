#ifndef XML_ANALYSIS_H
#define XML_ANALYSIS_H
#include <QXmlStreamReader>
#include <QtXml/QDomDocument>
#include <QObject>
//#include <QFrame>
#include <QFile>
#include "xsd_type.h"
#include "vector"
#include "type.h"
#include <QDebug>

class XmlAnalysis :public QObject
{
    Q_OBJECT
public:
    XmlAnalysis();
    XmlAnalysis(QString filename);
    QString file_name;
    void setfilename(QString filename);

    //节点
    XSDSystem *getSystemInfo(XSDSystem *system);
    Node *getNodeInfo(Node *node,QDomElement &element);
    XSDSubSystem *getSubSystemInfo(XSDSubSystem *subsystem, QDomElement &element);
    XSDDevice *getDeviceInfo(XSDDevice *dev,QDomElement &element);
    XSDSwitch *getSwitchInfo(XSDSwitch *switches,QDomElement &element);
    XSDNetworkInterface *getNetworkInterfaceInfo(XSDNetworkInterface *interface,QDomElement &element);

    //软件集合
    SoftwareSet *getSoftwareSetInfo(SoftwareSet *softwareset,QDomElement &element);
    XSDSoftWare *getSoftwareInfo(XSDSoftWare *software,QDomElement &element);

    //消息集合
    MsgSet *getMsgSetInfo(MsgSet *msgset,QDomElement &element);
    Msg *getMsgInfo(Msg *msg,QDomElement &element);
    MsgStruct *getMsgStructInfo(MsgStruct *msgstruct,QDomElement &element);
    MsgElement *getMsgElementInfo(MsgElement *msgelement,QDomElement &element);

    //接口关系集合
    InterfaceRelationSet *getInterfaceRelationSetInfo(InterfaceRelationSet *set,QDomElement &element);
    SoftwareConnection *getSoftwareConnectionInfo(SoftwareConnection *connection,QDomElement &element);
    SubSoftwareConnection *getSubSoftwareConnectionInfo(SubSoftwareConnection* connection,QDomElement &element);

    InterfaceRelation *getInterfaceRelationInfo(InterfaceRelation *relation,QDomElement &element);
    DDSInterfaceRelation *getDDSInterfaceRelationInfo(DDSInterfaceRelation *relation,QDomElement &element);
    PublishMember *getPublishMemberInfo(PublishMember *publish,QDomElement &element);
    SubscribeMember *getSubscribeMemberInfo(SubscribeMember *subscribe,QDomElement &element);
    DDSConfigure *getDDSConfigureInfo(DDSConfigure *conf,QDomElement &element);
    Configure *getConfigureInfo(Configure *conf,QDomElement &element);
    QosProfile *getQosProfileInfo(QosProfile *profile,QDomElement &element);

signals:
    void sendString(QString qstr);
};

#endif // XML_ANALYSIS_H
