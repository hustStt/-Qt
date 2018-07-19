#ifndef XSD_TYPE_H
#define XSD_TYPE_H
#include <QString>
#include <vector>
#pragma execution_character_set("utf-8")


const QString _XSDTransportType[] =
{
    "一次性传播",
    "周期性传播"
};

const QString _XSDSoftwareType[] =
{
    "计算软件",
    "显控软件"
};

const QString _XSDDDSEntity[] =
{
    "域参与者工厂",
    "域参与者",
    "发布者",
    "订阅者",
    "主题",
    "数据写者",
    "数据读者"
};

const QString _XSDQosStrategy[] =
{
    "ENTITY_FACTORY",
    "USER_DATA",
    "GROUP",
    "PRESENTATION",
    "PARTITION",
    "TOPIC_DATA",
    "DURABILITY",
};

const QString _XSDUnit[] =
{
    "毫秒",
    "微秒",
    "纳秒",
    "秒",
    "分钟",
    "小时",
    "天",
    "年",
    "月",
};

const QString _XSDNodeType[] =
{
    "计算设备",
    "交换设备",
    "存储设备",
    "显控设备",
    "子系统"
};

const QString _XSDMsgType[] =
{
    "DDS",
    "TCP",
    "UDP"
};

class XSDSoftWare
{
public:
    QString softid;
    QString name;
    QString type;       //_XSDNodeType
    QString domainid;
    QString portrange;

};

const QString CHS_XSDSoftWare[] =
{
    "软件标识",
    "名称",
    "类型",
    "域标识",
    "端口范围"
};

class XSDNetworkInterface
{
public:
    QString MACadress;
    QString IP;
    QString mask;
    QString gateway;
};

const QString CHS_XSDNetworkInterface[] =
{
    "MAC地址",
    "IP",
    "掩码",
    "网关"
};

class XSDDevice
{
public:
    QString id;
    QString name;
    QString type;   //_XSDNodeType
    std::vector <XSDNetworkInterface *> networkinterface_vec;
    //std::vector <XSDSoftWare *>software_vec;
    XSDDevice()
    {
        networkinterface_vec.clear();
        //software_vec.clear();
    }
};

const QString CHS_XSDDevice[] =
{
    "编号",
    "名称",
    "类型",
    "网络接口"
};

class XSDSwitch
{
public:
    QString id;
    QString name;
    std::vector <QString> ip_vec;
    std::vector <unsigned int> portnumber_vec;
    XSDSwitch()
    {
        ip_vec.clear();
        portnumber_vec.clear();
    }
};

const QString CHS_XSDSwitch[] =
{
    "编号",
    "名称",
    "IP",
    "端口"
};

class XSDSubSystem
{
public:
    QString id;
    QString name;
    QString multicast;
    std::vector <XSDDevice *> device_vec;
    std::vector <XSDSwitch *> switch_vec;
    std::vector <XSDSubSystem *> subsystem_vec;
    XSDSubSystem()
    {
        device_vec.clear();
        switch_vec.clear();
        subsystem_vec.clear();
    }
};

const QString CHS_XSDSubSystem[] =
{
    "编号",
    "名称",
    "组播地址",
    "设备",
    "交换机",
    "子系统"
};

class QosProfile
{
public:
    //待补充
};

class Configure
{
public:
    QosProfile *qosprofile;
    QString info;
    Configure()
    {
        qosprofile = nullptr;
    }
};

const QString CHS_Configure[] =
{
    "Qos策略",
    "说明"
};

class DDSConfigure
{
public:
    std::vector <Configure *> configure_vec;
    DDSConfigure()
    {
        configure_vec.clear();
    }
};

const QString CHS_DDSConfigure[] =
{
    "DDS配置"
};

class MsgElement
{
public:
    QString name;
    QString field_id;
    QString type;
    QString unnormaltypename;
    int maxlength = 0;
    QString id;
    QString info;
    QString precision;
    QString unit;//_XSDUnit
};

const QString CHS_MsgElement[] =
{
    "名称",
    "字段标识",
    "类型",
    "非基本类型名",
    "最大序列长度",
    "id",
    "说明",
    "精度",
    "单位"
};

class MsgStruct
{
public:
    std::vector <MsgElement *> element_vec;
    MsgStruct()
    {
        element_vec.clear();
    }
};

const QString CHS_MsgStruct[] =
{
    "成员"
};

class Msg
{
public:
    QString name;
    QString id;
    QString msgtype;//_XSDMsgType
    QString transporttype;//_XSDTransportType
    MsgStruct *msgstruct;
    Msg()
    {
        msgstruct = nullptr;
    }
};

const QString CHS_Msg[] =
{
    "名称",
    "标识",
    "消息类型",
    "传输类型",
    "结构"
};

class SubSoftwareConnection
{
public:
    QString device_id;
    QString software_id;
    QString info;
};

const QString CHS_SubSoftwareConnection[] =
{
    "设备编号",
    "软件标识",
    "说明"
};

class SoftwareConnection
{
public:
    std::vector <SubSoftwareConnection *> connection_vec;
    SoftwareConnection()
    {
        connection_vec.clear();
    }
};

const QString CHS_SoftwareConnection[] =
{
    "部署关系"
};

class PublishMember
{
public:
    QString softwareid;
    QString send_condition;
    QString data_rate;
    std::vector <DDSConfigure *> ddsconfigure_vec;
    PublishMember()
    {
        ddsconfigure_vec.clear();
    }
};

const QString CHS_PublichMember[] =
{
    "软件标识",
    "发送条件",
    "数据率",
    "配置"
};

class SubscribeMember
{
public:
    QString softwareid;
    QString receive_condition;
    std::vector <DDSConfigure *> ddsconfigure_vec;
    SubscribeMember()
    {
        ddsconfigure_vec.clear();
    }
};

const QString CHS_SubscribeMember[] =
{
    "软件标识",
    "接收条件",
    "配置"
};

class DDSInterfaceRelationNode
{
public:
    Msg *msg;
    std::vector <XSDSoftWare *> publish_vec;
    std::vector <XSDSoftWare *> subscribe_vec;
};

class DDSInterfaceRelation
{
public:
    QString domainid;
    std::vector <PublishMember *> publishmember_vec;
    std::vector <SubscribeMember *> subscribemember_vec;
    QString message;
    std::vector <DDSConfigure *> ddsconfigure_vec;
    DDSInterfaceRelation()
    {
        publishmember_vec.clear();
        subscribemember_vec.clear();
        ddsconfigure_vec.clear();
    }
};

const QString CHS_DDSInterfaceRelation[] =
{
    "域标识",
    "发布成员",
    "接收成员",
    "消息",
    "配置"
};

class Node
{
public:
    std::vector <XSDSubSystem *> subsystem_vec;
    Node()
    {
        subsystem_vec.clear();
    }
};

const QString CHS_Node[] =
{
    "成员"
};

class SoftwareSet
{
public:
    std::vector <XSDSoftWare *> software_vec;
    SoftwareSet()
    {
        software_vec.clear();
    }
};

const QString CHS_SoftwareSet[] =
{
    "软件"
};

class MsgSet
{
public:
    std::vector <Msg *> msg_vec;
    MsgSet()
    {
        msg_vec.clear();
    }
};

const QString CHS_MsgSet[] =
{
    "消息"
};

class InterfaceRelation
{
public:
    std::vector <DDSInterfaceRelation *> DDSinterfacerelation_vec;
    InterfaceRelation()
    {
        DDSinterfacerelation_vec.clear();
    }
};

const QString CHS_InterfaceRelation[] =
{
    "接口关系"
};

class InterfaceRelationSet
{
public:
    InterfaceRelation * interfacerelation;
    SoftwareConnection * softwareconnection;
    InterfaceRelationSet()
    {
        interfacerelation = nullptr;
        softwareconnection = nullptr;
    }
};

const QString CHS_InterfaceRelationSet[] =
{
    "DDS接口关系",
    "软件部署关系"
};

class XSDSystem
{
public:
    QString name;
    QString systemtype;
    std::vector <Node *>node_vec;
    SoftwareSet *softwareset;
    MsgSet *msgset;
    InterfaceRelationSet *interfacerelationset;
    XSDSystem()
    {
        node_vec.clear();
        softwareset = nullptr;
        msgset = nullptr;
        interfacerelationset = nullptr;
    }
};

const QString CHS_XSDSystem[]
{
    "名称",
    "类型",
    "节点",
    "软件集合",
    "消息集合",
    "接口关系集合"
};

struct changeName{
    QString oldName;
    QString newName;
};

#endif // XSD_TYPE_H
