#include "xml_analysis.h"

XmlAnalysis::XmlAnalysis()
{
    file_name = PATH + "test.xml";
}

XmlAnalysis::XmlAnalysis(QString filename)
{
    file_name = filename;
}

void XmlAnalysis::setfilename(QString filename)
{
    file_name = filename;
}

XSDSystem *XmlAnalysis::getSystemInfo(XSDSystem *system)
{
    QFile *file = new QFile(file_name);
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug()<<"open failed";
    QDomDocument doc;
    doc.setContent(file);
    for(int i = 0;i < doc.documentElement().childNodes().size();++i)
    {
        QDomElement element = doc.documentElement().childNodes().item(i).toElement();
        int j;
        for(j = 0;j < 6;++j)if(element.tagName() == CHS_XSDSystem[j])break;
        switch(j)
        {
            case 0://名称
            {
                system->name = element.text();
                sendString(system->name);
                break;
            }
            case 1://类型
            {
                system->systemtype = element.text();
                sendString(system->systemtype);
                break;
            }
            case 2://节点
            {
                sendString("");
                system->node_vec.push_back(new Node());
                getNodeInfo(system->node_vec[system->node_vec.size() - 1],element);
                break;
            }
            case 3://软件集合
            {
                system->softwareset = new SoftwareSet();
                getSoftwareSetInfo(system->softwareset,element);
                break;
            }
            case 4://消息集合
            {
                system->msgset = new MsgSet();
                getMsgSetInfo(system->msgset,element);
                break;
            }
            case 5://接口关系集合
            {
                system->interfacerelationset = new InterfaceRelationSet();
                getInterfaceRelationSetInfo(system->interfacerelationset,element);
                break;
            }
            default:break;
        }
    }
    file->close();
    free(file);
    return system;
}


Node * XmlAnalysis::getNodeInfo(Node * node, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        if(sub_element.tagName() == CHS_Node[0])
        {
            node->subsystem_vec.push_back(new XSDSubSystem());
            getSubSystemInfo(node->subsystem_vec[node->subsystem_vec.size() - 1],sub_element);
        }
    }
    return node;
}

XSDSubSystem *XmlAnalysis::getSubSystemInfo(XSDSubSystem *subsystem, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        int j;
        for(j = 0;j < 6;++j)if(sub_element.tagName() == CHS_XSDSubSystem[j])break;
        switch(j)
        {
            case 0://编号
            {
                subsystem->id = sub_element.text();
                sendString(subsystem->id);
                break;
            }
            case 1://名称
            {
                subsystem->name = sub_element.text();
                sendString(subsystem->name);
                break;
            }
            case 2://组播地址
            {
                subsystem->multicast = sub_element.text();
                sendString(subsystem->multicast);
                break;
            }
            case 3://设备
            {
                sendString("");
                subsystem->device_vec.push_back(new XSDDevice());
                getDeviceInfo(subsystem->device_vec[subsystem->device_vec.size() - 1],sub_element);
                break;
            }
            case 4://交换机
            {
                sendString("");
                subsystem->switch_vec.push_back(new XSDSwitch());
                getSwitchInfo(subsystem->switch_vec[subsystem->switch_vec.size() - 1],sub_element);
                break;
            }
            case 5://子系统
            {
                sendString("");
                subsystem->subsystem_vec.push_back(new XSDSubSystem());
                getSubSystemInfo(subsystem->subsystem_vec[subsystem->subsystem_vec.size() - 1],sub_element);
                break;
            }

            default:break;
        }

    }
    return subsystem;
}

XSDDevice *XmlAnalysis::getDeviceInfo(XSDDevice *dev, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        int j;
        for(j = 0;j < 4;++j)if(sub_element.tagName() == CHS_XSDDevice[j])break;
        switch(j)
        {
            case 0://编号
            {
                dev->id = sub_element.text();
                sendString(dev->id);
                break;
            }
            case 1://名称
            {
                dev->name = sub_element.text();
                sendString(dev->name);
                break;
            }
            case 2://类型
            {
                dev->type = sub_element.text();
                sendString(dev->type);
                break;
            }
            case 3://网络接口
            {
                dev->networkinterface_vec.push_back(new XSDNetworkInterface());
                getNetworkInterfaceInfo(dev->networkinterface_vec[dev->networkinterface_vec.size() - 1],sub_element);
                break;
            }
            default:break;
        }
    }
    return dev;
}

XSDSwitch *XmlAnalysis::getSwitchInfo(XSDSwitch *switches, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        int j;
        for(j = 0;j < 4;++j)if(sub_element.tagName() == CHS_XSDSwitch[j])break;
        switch(j)
        {
            case 0://编号
            {
                switches->id = sub_element.text();
                sendString(switches->id);
                break;
            }
            case 1://名称
            {
                switches->name = sub_element.text();
                sendString(switches->name);
                break;
            }
            case 2://IP
            {
                switches->ip_vec.push_back(sub_element.text());
                sendString(switches->ip_vec[switches->ip_vec.size() - 1]);
                break;
            }
            case 3://端口
            {
                switches->portnumber_vec.push_back(sub_element.text().toInt());
                sendString(QString::number(switches->portnumber_vec[switches->portnumber_vec.size() - 1]));
                break;
            }
            default:break;
        }
    }
    return switches;
}

XSDNetworkInterface *XmlAnalysis::getNetworkInterfaceInfo(XSDNetworkInterface *interface, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        int j;
        for(j = 0;j < 4;++j)if(sub_element.tagName() == CHS_XSDNetworkInterface[j])break;
        switch(j)
        {
            case 0://mac
            {
                interface->MACadress = sub_element.text();
                sendString(interface->MACadress);
                break;
            }
            case 1://ip
            {
                interface->IP = sub_element.text();
                sendString(interface->IP);
                break;
            }
            case 2://掩码
            {
                interface->mask = sub_element.text();
                sendString(interface->mask);
                break;
            }
            case 3://网关
            {
                interface->gateway = sub_element.text();
                sendString(interface->gateway);
                break;
            }
            default:break;
        }
    }
    return interface;
}

SoftwareSet *XmlAnalysis::getSoftwareSetInfo(SoftwareSet *softwareset, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        if(sub_element.tagName() == CHS_SoftwareSet[0])
        {
            sendString("");
            softwareset->software_vec.push_back(new XSDSoftWare());
            getSoftwareInfo(softwareset->software_vec[softwareset->software_vec.size() - 1],sub_element);
        }
    }
    return softwareset;
}

XSDSoftWare *XmlAnalysis::getSoftwareInfo(XSDSoftWare *software, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        int j;
        for(j = 0;j < 5;++j)if(sub_element.tagName() == CHS_XSDSoftWare[j])break;
        switch(j)
        {
            case 0://软件标识
            {
                software->softid = sub_element.text();
                sendString(software->softid);
                break;
            }
            case 1://名称
            {
                software->name = sub_element.text();
                sendString(software->name);
                break;
            }
            case 2://类型
            {
                software->type = sub_element.text();
                sendString(software->type);
                break;
            }
            case 3://域标识
            {
                software->domainid = sub_element.text();
                sendString(software->domainid);
                break;
            }
            case 4://端口范围
            {
                software->portrange = sub_element.text();
                sendString(software->portrange);
                break;
            }
            default:break;
        }
    }
    return software;
}

MsgSet *XmlAnalysis::getMsgSetInfo(MsgSet *msgset, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        if(sub_element.tagName() == CHS_MsgSet[0])
        {
            sendString("");
            msgset->msg_vec.push_back(new Msg());
            getMsgInfo(msgset->msg_vec[msgset->msg_vec.size() - 1],sub_element);
        }
    }
    return msgset;
}

Msg *XmlAnalysis::getMsgInfo(Msg *msg, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        int j;
        for(j = 0;j < 5;++j)if(sub_element.tagName() == CHS_Msg[j])break;
        switch(j)
        {
            case 0://名称
            {
                msg->name = sub_element.text();
                sendString(msg->name);
                break;
            }
            case 1://标识
            {
                msg->id = sub_element.text();
                sendString(msg->id);
                break;
            }
            case 2://消息类型
            {
                msg->msgtype = sub_element.text();
                sendString(msg->msgtype);
                break;
            }
            case 3://传输类型
            {
                msg->transporttype = sub_element.text();
                sendString(msg->transporttype);
                break;
            }
            case 4://结构
            {
                msg->msgstruct = new MsgStruct();
                getMsgStructInfo(msg->msgstruct,sub_element);
                break;
            }
            default:break;
        }
    }
    return msg;
}

MsgStruct *XmlAnalysis::getMsgStructInfo(MsgStruct *msgstruct, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        if(sub_element.tagName() == CHS_MsgStruct[0])
        {
            sendString("");
            msgstruct->element_vec.push_back(new MsgElement());
            getMsgElementInfo(msgstruct->element_vec[msgstruct->element_vec.size() - 1],sub_element);
        }
    }
    return msgstruct;
}

MsgElement *XmlAnalysis::getMsgElementInfo(MsgElement *msgelement, QDomElement &element)
{
    for(int i = 0;i < element.attributes().size();++i)
    {
        int j;
        for(j = 0;j < 9;++j)if(element.attributes().item(i).nodeName() == CHS_MsgElement[j])break;
        switch(j)
        {
            case 0://名称
            {
                msgelement->name = element.attributes().item(i).nodeValue();
                sendString(msgelement->name);
                break;
            }
            case 1://字段标识
            {
                msgelement->field_id = element.attributes().item(i).nodeValue();
                sendString(msgelement->field_id);
                break;
            }
            case 2://类型
            {
                msgelement->type = element.attributes().item(i).nodeValue();
                sendString(msgelement->type);
                break;
            }
            case 3://非基本类型名
            {
                msgelement->unnormaltypename = element.attributes().item(i).nodeValue();
                sendString(msgelement->unnormaltypename);
                break;
            }
            case 4://最大序列长度
            {
                msgelement->maxlength = element.attributes().item(i).nodeValue().toInt();
                sendString(QString::number(msgelement->maxlength));
                break;
            }
            case 5://id
            {
                msgelement->id = element.attributes().item(i).nodeValue();
                sendString(msgelement->id);
                break;
            }
            case 6://说明
            {
                msgelement->info = element.attributes().item(i).nodeValue();
                sendString(msgelement->info);
                break;
            }
            case 7://精度
            {
                msgelement->precision = element.attributes().item(i).nodeValue();
                sendString(msgelement->precision);
                break;
            }
            case 8://单位
            {
                msgelement->unit = element.attributes().item(i).nodeValue();
                sendString(msgelement->unit);
                break;
            }

            default:break;
        }
    }
    return msgelement;
}

InterfaceRelationSet *XmlAnalysis::getInterfaceRelationSetInfo(InterfaceRelationSet *set, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        int j;
        for(j = 0;j < 2;++j)if(sub_element.tagName() == CHS_InterfaceRelationSet[j])break;
        switch(j)
        {
            case 0://DDS接口关系
            {
                set->interfacerelation = new InterfaceRelation();
                getInterfaceRelationInfo(set->interfacerelation,sub_element);
                break;
            }
            case 1://软件部署类型
            {
                set->softwareconnection = new SoftwareConnection();
                getSoftwareConnectionInfo(set->softwareconnection,sub_element);
                break;
            }
            default:break;
        }
    }
    return set;
}

SoftwareConnection *XmlAnalysis::getSoftwareConnectionInfo(SoftwareConnection *connection, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        if(sub_element.tagName() == CHS_SoftwareConnection[0])
        {
            sendString("");
            connection->connection_vec.push_back(new SubSoftwareConnection());
            getSubSoftwareConnectionInfo(connection->connection_vec[connection->connection_vec.size() - 1],sub_element);
        }
    }
    return connection;
}

SubSoftwareConnection *XmlAnalysis::getSubSoftwareConnectionInfo(SubSoftwareConnection *connection, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        int j;
        for(j = 0;j < 3;++j)if(sub_element.tagName() == CHS_SubSoftwareConnection[j])break;
        switch(j)
        {
            case 0://设备编号
            {
                connection->device_id = sub_element.text();
                sendString(connection->device_id);
                break;
            }
            case 1://软件标识
            {
                connection->software_id = sub_element.text();
                sendString(connection->software_id);
                break;
            }
            case 2://说明
            {
                connection->info = sub_element.text();
                sendString(connection->info);
                break;
            }
            default:break;
        }
    }
    return connection;
}

InterfaceRelation *XmlAnalysis::getInterfaceRelationInfo(InterfaceRelation *relation, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        if(sub_element.tagName() == CHS_InterfaceRelation[0])
        {
            sendString("");
            relation->DDSinterfacerelation_vec.push_back(new DDSInterfaceRelation());
            getDDSInterfaceRelationInfo(relation->DDSinterfacerelation_vec[relation->DDSinterfacerelation_vec.size() - 1],sub_element);
        }
    }
    return relation;
}

DDSInterfaceRelation *XmlAnalysis::getDDSInterfaceRelationInfo(DDSInterfaceRelation *relation, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        int j;
        for(j = 0;j < 5;++j)if(sub_element.tagName() == CHS_DDSInterfaceRelation[j])break;
        switch(j)
        {
            case 0://域标识
            {
                relation->domainid = sub_element.text();
                sendString(relation->domainid);
                break;
            }
            case 1://发布成员
            {
                relation->publishmember_vec.push_back(new PublishMember());
                getPublishMemberInfo(relation->publishmember_vec[relation->publishmember_vec.size() - 1],sub_element);
                break;
            }
            case 2://接受成员
            {
                relation->subscribemember_vec.push_back(new SubscribeMember());
                getSubscribeMemberInfo(relation->subscribemember_vec[relation->subscribemember_vec.size() - 1],sub_element);
                break;
            }
            case 3://消息
            {
                relation->message = sub_element.text();
                sendString(relation->message);
                break;
            }
            case 4://配置
            {
                relation->ddsconfigure_vec.push_back(new DDSConfigure());
                getDDSConfigureInfo(relation->ddsconfigure_vec[relation->ddsconfigure_vec.size() - 1],sub_element);
                break;
            }
            default:break;
        }
    }
    return relation;
}

PublishMember *XmlAnalysis::getPublishMemberInfo(PublishMember *publish, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        int j;
        for(j = 0;j < 4;++j)if(sub_element.tagName() == CHS_PublichMember[j])break;
        switch(j)
        {
            case 0://软件标识
            {
                publish->softwareid = sub_element.text();
                sendString(publish->softwareid);
                break;
            }
            case 1://发送条件
            {
                publish->send_condition = sub_element.text();
                sendString(publish->send_condition);
                break;
            }
            case 2://数据率
            {
                publish->data_rate = sub_element.text();
                sendString(publish->data_rate);
                break;
            }
            case 3://配置
            {
                publish->ddsconfigure_vec.push_back(new DDSConfigure());
                getDDSConfigureInfo(publish->ddsconfigure_vec[publish->ddsconfigure_vec.size() - 1],sub_element);
                break;
            }
            default:break;
        }
    }
    return publish;
}

SubscribeMember *XmlAnalysis::getSubscribeMemberInfo(SubscribeMember *subscribe, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        int j;
        for(j = 0;j < 3;++j)if(sub_element.tagName() == CHS_SubscribeMember[j])break;
        switch(j)
        {
            case 0://软件标识
            {
                subscribe->softwareid = sub_element.text();
                sendString(subscribe->softwareid);
                break;
            }
            case 1://接受条件
            {
                subscribe->receive_condition = sub_element.text();
                sendString(subscribe->receive_condition);
                break;
            }
            case 2://配置
            {
                subscribe->ddsconfigure_vec.push_back(new DDSConfigure());
                getDDSConfigureInfo(subscribe->ddsconfigure_vec[subscribe->ddsconfigure_vec.size() - 1],sub_element);
                break;
            }
            default:break;
        }
    }
    return subscribe;
}

DDSConfigure *XmlAnalysis::getDDSConfigureInfo(DDSConfigure *conf, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        if(sub_element.tagName() == CHS_DDSConfigure[0])
        {
            sendString("");
            conf->configure_vec.push_back(new Configure());
            getConfigureInfo(conf->configure_vec[conf->configure_vec.size() - 1],sub_element);
        }
    }
    return conf;
}

Configure *XmlAnalysis::getConfigureInfo(Configure *conf, QDomElement &element)
{
    for(int i = 0;i < element.childNodes().size();++i)
    {
        QDomElement sub_element = element.childNodes().item(i).toElement();
        int j;
        for(j = 0;j < 2;++j)if(sub_element.tagName() == CHS_Configure[j])break;
        switch(j)
        {
            case 0://Qos策略
            {
                conf->qosprofile = new QosProfile();
                getQosProfileInfo(conf->qosprofile,sub_element);
                break;
            }
            case 1://说明
            {
                conf->info = sub_element.text();
                sendString(conf->info);
                break;
            }
            default:break;
        }
    }
    return conf;
}

QosProfile *XmlAnalysis::getQosProfileInfo(QosProfile *profile, QDomElement &element)
{
    //待补充Qos
    return NULL;
}
