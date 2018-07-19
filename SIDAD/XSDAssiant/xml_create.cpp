#include "xml_create.h"
#include <QTextStream>

void XmlCreate::setfilename(QString filename)
{
    file_name = filename;
}

XSDSystem *XmlCreate::setSystemInfo(XSDSystem *xsdsystem)
{
    doc.clear();
    QFile *file = new QFile(file_name);
    if(!file)
        qDebug()<<"there is no this file "<<file_name;
    if(!file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        qDebug()<<"open failed";
    //添加xml头部
    QDomElement root = doc.createElement("系统");
    root.setAttribute("xsi:noNamespaceSchemaLocation","接口协议模型.xsd");
    root.setAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
    doc.appendChild(root);
    std::vector <QDomElement> element_vec;
    element_vec.clear();
    for(int i = 0;i < 2;++i)
    {
        element_vec.push_back(doc.createElement(CHS_XSDSystem[i]));
        element_vec[element_vec.size() - 1].appendChild(doc.createTextNode(*(&xsdsystem->name + i)));
    }
    for(unsigned i = 0;i < xsdsystem->node_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_XSDSystem[2]));
        setNodeInfo(xsdsystem->node_vec[i],element_vec[element_vec.size() - 1]);
    }
    if(xsdsystem->softwareset)
    {
        element_vec.push_back(doc.createElement(CHS_XSDSystem[3]));
        setSoftwareSetInfo(xsdsystem->softwareset,element_vec[element_vec.size() - 1]);
    }
    else
        xsdsystem->softwareset = new SoftwareSet();
    if(xsdsystem->msgset)
    {
        element_vec.push_back(doc.createElement(CHS_XSDSystem[4]));
        setMsgSetInfo(xsdsystem->msgset,element_vec[element_vec.size() - 1]);
    }
    else
        xsdsystem->msgset = new MsgSet();
    if(xsdsystem->interfacerelationset)
    {
        element_vec.push_back(doc.createElement(CHS_XSDSystem[5]));
        setInterfaceRelationSetInfo(xsdsystem->interfacerelationset,element_vec[element_vec.size() - 1]);
    }
    else
        xsdsystem->interfacerelationset = new InterfaceRelationSet();
    for(unsigned i = 0;i < element_vec.size();++i)
        root.appendChild(element_vec[i]);
    QTextStream out(file);
    out.setCodec("UTF-8");
    doc.save(out,4,QDomNode::EncodingFromTextStream);
    file->close();
    //free(file);
    //system("gvim " + file_name.toLatin1());
    return xsdsystem;
}


Node * XmlCreate::setNodeInfo(Node * node, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();
    for(unsigned i = 0;i < node->subsystem_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_Node[0]));
        setSubSystemInfo(node->subsystem_vec[i],element_vec[element_vec.size() - 1]);
    }
    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);

    return node;
}

XSDSubSystem *XmlCreate::setSubSystemInfo(XSDSubSystem *subsystem, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();

    for(int i = 0;i < 3;++i)
    {
        element_vec.push_back(doc.createElement(CHS_XSDSubSystem[i]));
        element_vec[element_vec.size() - 1].appendChild(doc.createTextNode(*(&subsystem->id + i)));
    }

    for(unsigned i = 0;i < subsystem->device_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_XSDSubSystem[3]));
        setDeviceInfo(subsystem->device_vec[i],element_vec[element_vec.size() - 1]);
    }

    for(unsigned i = 0;i < subsystem->switch_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_XSDSubSystem[4]));
        setSwitchInfo(subsystem->switch_vec[i],element_vec[element_vec.size() - 1]);
    }

    for(unsigned i = 0;i < subsystem->subsystem_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_XSDSubSystem[5]));
        setSubSystemInfo(subsystem->subsystem_vec[i],element_vec[element_vec.size() - 1]);
    }

    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return subsystem;
}

XSDDevice *XmlCreate::setDeviceInfo(XSDDevice *dev, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();

    for(int i = 0;i < 3;++i)
    {
        element_vec.push_back(doc.createElement(CHS_XSDDevice[i]));
        element_vec[element_vec.size() - 1].appendChild(doc.createTextNode(*(&dev->id + i)));
    }

    for(unsigned i = 0;i < dev->networkinterface_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_XSDDevice[3]));
        setNetworkInterfaceInfo(dev->networkinterface_vec[i],element_vec[element_vec.size() - 1]);
    }

    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return dev;
}

XSDSwitch *XmlCreate::setSwitchInfo(XSDSwitch *switches, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();

    for(int i = 0;i < 2;++i)
    {
        element_vec.push_back(doc.createElement(CHS_XSDSwitch[i]));
        element_vec[element_vec.size() - 1].appendChild(doc.createTextNode(*(&switches->id + i)));
    }

    for(unsigned i = 0;i < switches->ip_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_XSDSwitch[2]));
        element_vec[element_vec.size() - 1].appendChild(doc.createTextNode(switches->ip_vec[i]));
    }

    for(unsigned i = 0;i < switches->portnumber_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_XSDSwitch[3]));
        element_vec[element_vec.size() - 1].appendChild(doc.createTextNode(QString::number(switches->portnumber_vec[i])));
    }

    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return switches;
}

XSDNetworkInterface *XmlCreate::setNetworkInterfaceInfo(XSDNetworkInterface *interface, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();

    for(int i = 0;i < 4;++i)
    {
        element_vec.push_back(doc.createElement(CHS_XSDNetworkInterface[i]));
        element_vec[element_vec.size() - 1].appendChild(doc.createTextNode(*(&interface->MACadress + i)));
        element.appendChild(element_vec[i]);
    }

    return interface;
}

SoftwareSet *XmlCreate::setSoftwareSetInfo(SoftwareSet *softwareset, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();

    for(unsigned i = 0;i < softwareset->software_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_SoftwareSet[0]));
        setSoftwareInfo(softwareset->software_vec[i],element_vec[element_vec.size() - 1]);
    }

    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return softwareset;
}

XSDSoftWare *XmlCreate::setSoftwareInfo(XSDSoftWare *software, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();

    for(int i = 0;i < 5;++i)
    {
        element_vec.push_back(doc.createElement(CHS_XSDSoftWare[i]));
        element_vec[element_vec.size() - 1].appendChild(doc.createTextNode(*(&software->softid + i)));
        element.appendChild(element_vec[i]);
    }
    return software;
}

MsgSet *XmlCreate::setMsgSetInfo(MsgSet *msgset, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();

    for(unsigned i = 0;i < msgset->msg_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_MsgSet[0]));
        setMsgInfo(msgset->msg_vec[i],element_vec[element_vec.size() - 1]);
    }

    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return msgset;
}

Msg *XmlCreate::setMsgInfo(Msg *msg, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();

    for(int i = 0;i < 4;++i)
    {
        element_vec.push_back(doc.createElement(CHS_Msg[i]));
        element_vec[element_vec.size() - 1].appendChild(doc.createTextNode(*(&msg->name + i)));
    }

    if(msg->msgstruct)
    {
        element_vec.push_back(doc.createElement(CHS_Msg[4]));
        setMsgStructInfo(msg->msgstruct,element_vec[element_vec.size() - 1]);
    }
    else
        msg->msgstruct = new MsgStruct();
    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return msg;
}

MsgStruct *XmlCreate::setMsgStructInfo(MsgStruct *msgstruct, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();

    for(unsigned i = 0;i < msgstruct->element_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_MsgStruct[0]));
        setMsgElementInfo(msgstruct->element_vec[i],element_vec[element_vec.size() - 1]);
    }

    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return msgstruct;
}

MsgElement *XmlCreate::setMsgElementInfo(MsgElement *msgelement, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();
    for(int i = 0; i < 4;++i)
    {
        if(*(&msgelement->name + i) != 0)
            element.setAttribute(CHS_MsgElement[i],*(&msgelement->name + i));
    }
    if(msgelement->maxlength)
        element.setAttribute(CHS_MsgElement[4],msgelement->maxlength);
    for(int i = 0; i < 4;++i)
    {
        if(*(&msgelement->id + i) != 0)
            element.setAttribute(CHS_MsgElement[5 + i],*(&msgelement->id + i));
    }
    return msgelement;
}

InterfaceRelationSet *XmlCreate::setInterfaceRelationSetInfo(InterfaceRelationSet *set, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();
    if(set->interfacerelation)
    {
        element_vec.push_back(doc.createElement(CHS_InterfaceRelationSet[0]));
        setInterfaceRelationInfo(set->interfacerelation,element_vec[element_vec.size() - 1]);
    }
    else
        set->interfacerelation = new InterfaceRelation();
    if(set->softwareconnection)
    {
        element_vec.push_back(doc.createElement(CHS_InterfaceRelationSet[1]));
        setSoftwareConnectionInfo(set->softwareconnection,element_vec[element_vec.size() - 1]);
    }
    else
        set->softwareconnection = new SoftwareConnection();

    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return set;
}


SoftwareConnection *XmlCreate::setSoftwareConnectionInfo(SoftwareConnection *connection, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();
    for(unsigned i = 0;i < connection->connection_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_SoftwareConnection[0]));
        setSubSoftwareConnectionInfo(connection->connection_vec[i],element_vec[element_vec.size() - 1]);
    }
    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return connection;
}

SubSoftwareConnection *XmlCreate::setSubSoftwareConnectionInfo(SubSoftwareConnection *connection, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();
    for(int i = 0;i < 3;++i)
    {
        element_vec.push_back(doc.createElement(CHS_SubSoftwareConnection[i]));
        element_vec[element_vec.size() - 1].appendChild(doc.createTextNode(*(&connection->device_id + i)));
    }

    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return connection;
}

InterfaceRelation *XmlCreate::setInterfaceRelationInfo(InterfaceRelation *relation, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();

    for(unsigned i = 0;i < relation->DDSinterfacerelation_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_InterfaceRelation[0]));
        setDDSInterfaceRelationInfo(relation->DDSinterfacerelation_vec[i],element_vec[element_vec.size() - 1]);
    }

    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return relation;
}

DDSInterfaceRelation *XmlCreate::setDDSInterfaceRelationInfo(DDSInterfaceRelation *relation, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();

    element_vec.push_back(doc.createElement(CHS_DDSInterfaceRelation[0]));
    element_vec[element_vec.size() - 1].appendChild(doc.createTextNode(relation->domainid));

    for(unsigned i = 0;i < relation->publishmember_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_DDSInterfaceRelation[1]));
        setPublishMemberInfo(relation->publishmember_vec[i],element_vec[element_vec.size() - 1]);
    }

    for(unsigned i = 0;i < relation->subscribemember_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_DDSInterfaceRelation[2]));
        setSubscribeMemberInfo(relation->subscribemember_vec[i],element_vec[element_vec.size() - 1]);
    }


    element_vec.push_back(doc.createElement(CHS_DDSInterfaceRelation[3]));
    element_vec[element_vec.size() - 1].appendChild(doc.createTextNode(relation->message));

    for(unsigned i = 0;i < relation->ddsconfigure_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_DDSInterfaceRelation[4]));
        setDDSConfigureInfo(relation->ddsconfigure_vec[i],element_vec[element_vec.size() - 1]);
    }

    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return relation;
}

PublishMember *XmlCreate::setPublishMemberInfo(PublishMember *publish, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();
    for(int i = 0;i < 3;++i)
    {
        element_vec.push_back(doc.createElement(CHS_PublichMember[i]));
        element_vec[element_vec.size() - 1].appendChild(doc.createTextNode(*(&publish->softwareid + i)));
    }

    for(unsigned i = 0;i < publish->ddsconfigure_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_PublichMember[3]));
        setDDSConfigureInfo(publish->ddsconfigure_vec[i],element_vec[element_vec.size() - 1]);
    }

    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return publish;
}

SubscribeMember *XmlCreate::setSubscribeMemberInfo(SubscribeMember *subscribe, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();
    for(int i = 0;i < 2;++i)
    {
        element_vec.push_back(doc.createElement(CHS_SubscribeMember[i]));
        element_vec[element_vec.size() - 1].appendChild(doc.createTextNode(*(&subscribe->softwareid + i)));
    }

    for(unsigned i = 0;i < subscribe->ddsconfigure_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_SubscribeMember[2]));
        setDDSConfigureInfo(subscribe->ddsconfigure_vec[i],element_vec[element_vec.size() - 1]);
    }

    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return subscribe;
}

DDSConfigure *XmlCreate::setDDSConfigureInfo(DDSConfigure *conf, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();

    for(unsigned i = 0;i < conf->configure_vec.size();++i)
    {
        element_vec.push_back(doc.createElement(CHS_DDSConfigure[0]));
        setConfigureInfo(conf->configure_vec[i],element_vec[element_vec.size() - 1]);
    }

    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return conf;
}

Configure *XmlCreate::setConfigureInfo(Configure *conf, QDomElement &element)
{
    std::vector <QDomElement> element_vec;
    element_vec.clear();

    if(conf->qosprofile)
    {
        element_vec.push_back(doc.createElement(CHS_Configure[0]));
        setQosProfileInfo(conf->qosprofile,element_vec[element_vec.size() - 1]);
    }
    else
        conf->qosprofile = new QosProfile();

    element_vec.push_back(doc.createElement(CHS_Configure[1]));
    element_vec[element_vec.size() - 1].appendChild(doc.createTextNode(conf->info));


    for(unsigned i = 0;i < element_vec.size();++i)
        element.appendChild(element_vec[i]);
    return conf;
}

QosProfile *XmlCreate::setQosProfileInfo(QosProfile *profile, QDomElement &element)
{
    //待补充Qos
    return NULL;
}

