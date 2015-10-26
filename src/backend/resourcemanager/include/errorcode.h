#ifndef RESOURCE_MANAGER_ERROR_CODE_H
#define RESOURCE_MANAGER_ERROR_CODE_H

#include "envswitch.h"

#define ERROR_MESSAGE_RESERVE_SIZE			256
#define ERROR_HEAD_MESSAGE_RESERVE_SIZE		88

enum DRM_ERROR_CODE {
	FUNC_RETURN_OK = 0,
	FUNC_RETURN_FAIL,
	FUNC_NOT_EXECUTED,
    INSUFFICIENT_MEMORY_TO_ALLOC,
    SYSTEM_CALL_ERROR,
	TRANSCANCEL_INPROGRESS,
	LIBPQ_CONN_ERROR,
	LIBPQ_FAIL_EXECUTE,

    /*-----------------------------------------------------------------------*/
    MAIN_START_TAG = 20,
    MAIN_INTERNAL_LOGICERROR,		/* Should never occur. */
    MAIN_UNSET_GLOBALCONFIG,		/* Unset global configure file. */
    MAIN_UNSET_HAWQSITECONFIG,
    MAIN_WRONG_GLOBALCONFIG,
    MAIN_WRONG_COMMANDLINE,
	MAIN_WRONG_ENV_VAR,
	MAIN_CONF_UNSET_SEGMENT_MEMORY_USE,
	MAIN_CONF_UNSET_SEGMENT_CORE_USE,
    MAIN_CONF_UNSET_ROLE,
    MAIN_CONF_UNSET_MASTER_ADDR_DOMAINSOCKET_PORT,
    MAIN_CONF_UNSET_MASTER_ADDR,
    MAIN_CONF_UNSET_STANDBY_ADDR_DOMAINSOCKET_PORT,
    MAIN_CONF_UNSET_STANDBY_ADDR,
    MAIN_CONF_UNSET_SEGMENT_PORT,
    MAIN_CONF_UNSET_TEMPDIRS,
    MAIN_CONF_UNSET_ENFORCER_CPU_FLAG,
	MAIN_CONF_UNSET_ENFORCER_MEMORY_FLAG,
	MAIN_CONF_UNSET_ENFORCER_BLKIO_FLAG,
	MAIN_CONF_UNSET_ENFORCER_CGROUP_MOUNT_POINT,
	MAIN_CONF_UNSET_ENFORCER_CGROUP_HIERARCHY_NAME,
	MAIN_CONF_UNSET_ENFORCER_VCORE_PCORE_RATIO,
	MAIN_CONF_UNSET_ENFORCER_VMEMO_PMEMO_RATIO,
	MAIN_CONF_UNSET_ENFORCER_VDISK_PDISK_RATIO,

	/*-----------------------------------------------------------------------*/
	RESQUEMGR_START_TAG = 100,
	RESQUEMGR_WRONG_ATTRNAME,
	RESQUEMGR_DUPLICATE_ATTRNAME,
	RESQUEMGR_NO_QUEID,				/* Resource queue id does not exist. */
	RESQUEMGR_NO_QUENAME,
	RESQUEMGR_NO_ASSIGNEDQUEUE,
	RESQUEMGR_DUPLICATE_QUEID,
	RESQUEMGR_DUPLICATE_QUENAME,
	RESQUEMGR_IN_USE,				/* Resource queue is in use. */
	RESQUEMGR_HAS_ROLES_ASSIGNED,	/* Resource queue has at least 1 role/user assigned to. */
	RESQUEMGR_DUPLICATE_USERID,		/* Add duplicate user id to resource queue manager. */
	RESQUEMGR_NO_USERID,			/* Remove or update user id that does not exist. */
	RESQUEMGR_EXCEED_MAX_QUEUE_NUMBER, /* Exceed max resource queue number */
	RESQUEMGR_NO_USERNAME,
	RESQUEMGR_NOT_SUPERUSER,
	RESQUEMGR_PARALLEL_FULL,		/* Can not accept more connections. */
	RESQUEMGR_LACK_ATTR,
	RESQUEMGR_WRONG_ATTR,
	RESQUEMGR_WRONG_PARENT_QUEUE,
	RESQUEMGR_WRONG_TARGET_QUEUE,
	RESQUEMGR_INCONSISTENT_RESOURCE_EXP,
	RESQUEMGR_MIXED_VER1VER2_ATTR,
	RESQUEMGR_NO_RESOURCE,
	RESQUEMGR_NO_QUE_IN_RATIO,
	RESQUEMGR_NO_RATIO,
	RESQUEMGR_TOO_MANY_FIXED_SEGNUM,
	RESQUEMGR_DUPLICATE_SESSIONID,
	RESQUEMGR_NO_SESSIONID,
	RESQUEMGR_DEADLOCK_DETECTED,
	RESQUEMGR_NOCLUSTER_TIMEOUT,
	RESQUEMGR_NORESOURCE_TIMEOUT,
	RESQUEMGR_WRONG_RES_QUOTA_EXP,

	REQUESTHANDLER_START_TAG = 200,
	REQUESTHANDLER_WAIT_RESOURCE,
	REQUESTHANDLER_INTERNAL_LOGICERROR,

	REQUESTHANDLER_RECV_ERROR,
	REQUESTHANDLER_RECV_REMOTE_CLOSE,
	REQUESTHANDLER_SEND_ERROR,

	REQUESTHANDLER_FAIL_START_SOCKET_SERVER,
	REQUESTHANDLER_FAIL_SELECT_SOCKET_SERVER,
	REQUESTHANDLER_FAIL_ACCEPT_SOCKET_CONNECTION,
	REQUESTHANDLER_FAIL_CREATE_THREAD,
	REQUESTHANDLER_WRONG_MESSAGE_CONTENT,
	REQUESTHANDLER_WRONG_CONNSTAT,

	REQUESTHANDLER_WRONGFORMAT_STARTTAG,
	REQUESTHANDLER_WRONGFORMAT_ENDTAG,
	REQUESTHANDLER_WRONGENCODING,
	REQUESTHANDLER_WRONGMESSAGE,

	/*-----------------------------------------------------------------------*/
	CONNTRACK_START_TAG = 300,
	CONNTRACK_NO_CONNID,
	CONNTRACK_CONNID_FULL,
	CONNTRACK_DUPLICATE_CONNID,

	/*-----------------------------------------------------------------------*/
	RESOURCEPOOL_START_TAG = 400,
	RESOURCEPOOL_INTERNAL_NO_HOST_INDEX,
	RESOURCEPOOL_INTERNAL_DUPLICATE_HOST,
	RESOURCEPOOL_NO_HOSTID,
	RESOURCEPOOL_NO_RATIO,
	RESOURCEPOOL_DUPLICATE_RATIO,
	RESOURCEPOOL_ERROR_ADD_RESOURCE_GROUP,
	RESOURCEPOOL_NO_GROUPID,
	RESOURCEPOOL_DUPLICATE_HOST,
	RESOURCEPOOL_UNRESOLVED_HOST,
	RESOURCEPOOL_TOO_MANY_UAVAILABLE_HOST,

	/*-----------------------------------------------------------------------*/
	COMM2RM_CLIENT_START_TAG = 500,
	COMM2RM_CLIENT_FAIL_CONN,
	COMM2RM_CLIENT_FAIL_SEND,
	COMM2RM_CLIENT_FAIL_RECV,
	COMM2RM_CLIENT_WRONG_INPUT,
	COMM2RM_CLIENT_FULL_RESOURCECONTEXT,

	/*-----------------------------------------------------------------------*/
	RM2GRM_START_TAG = 600,
	RM2GRM_WRONG_GLOB_RM_TYPE,
	RM2GRM_FAIL_CONNECT_GLOB_RM,
	RM2GRM_FAIL_CREATE_JOB,
	RM2GRM_FAIL_ALLOCATE_RESOURCE,

	/*-----------------------------------------------------------------------*/
	RESBROK_START_TAG = 700,
	RESBROK_LACK_PARAMETER,
	RESBROK_WRONG_PARAMETER,
	RESBROK_WRONG_CONFIGURE_FILEPATH,
	RESBROK_WRONG_CONFIGURE_SERVERTYPE,
	RESBROK_WRONG_GLOB_MGR_ADDRESS,
	RESBROK_WRONG_GLOB_MGR_QUEUE,
	RESBROK_WRONG_GLOB_MGR_APPNAME,
	RESBROK_PIPE_ERROR,
	RESBROK_NOMORE_RESOURCE_IN_GRM,
	RESBROK_WRONG_MESSAGE_ID,
	RESBROK_ERROR_GRM,
	RESBROK_TEMP_NO_RESOURCE,
	RESBROK_PIPE_BUSY,
	RESBROK_FAIL_ALLOCATE_RESOURCE,

	RMSEG_START_TAG = 800,
	RMSEG_STATUS_BAD_HOSTINFO,
	RM_STATUS_BAD_TMPDIR,
    RM_STATUS_BAD_DUMP_TYPE,

	RMDDL_START_TAG = 900,
	RMDDL_WRONG_ATTRNAME,
	RMDDL_WRONG_ATTRVALUE,

	/*-----------------------------------------------------------------------*/
	RESENFORCER_START_TAG = 1000,
	RESENFORCER_FAIL_FIND_CGROUP_HASH_ENTRY,
	RESENFORCER_FAIL_CREATE_CGROUP_HASH_ENTRY,
	RESENFORCER_FAIL_DELETE_CGROUP_HASH_ENTRY,
	RESENFORCER_FAIL_CREATE_CGROUP,
	RESENFORCER_FAIL_DELETE_CGROUP,
	RESENFORCER_FAIL_READ_CGROUP_FILE,
	RESENFORCER_FAIL_WRITE_CGROUP_FILE,
	RESENFORCER_FAIL_UPDATE_MEMORY_QUOTA,
	RESENFORCER_ERROR_INSUFFICIENT_MEMORY,

	/*-----------------------------------------------------------------------*/
	ASYNCCOMM_START_TAG = 1100,
	ASYNCCOMM_BUFFER_ARRAY_FULL,

	/*-----------------------------------------------------------------------*/
	UTIL_PROPERTIES_START_TAG = 1200,
	UTIL_PROPERTIES_NO_FILE,
	UTIL_PROPERTIES_INVALID_XML,
	UTIL_PROPERTIES_ELEMENT_INTERNAL_ERROR,
	UTIL_PROPERTIES_ELEMENT_MULTI_CONFIGURATION,
	UTIL_PROPERTIES_ELEMENT_WRONG_CONFIGURATION,
	UTIL_PROPERTIES_ELEMENT_WRONG_PROPERTY,
	UTIL_PROPERTIES_ELEMENT_WRONG_NAME,
	UTIL_PROPERTIES_ELEMENT_WRONG_VALUE,
	UTIL_PROPERTIES_ELEMENT_WRONG_DESC,
	UTIL_PROPERTIES_NO_KEY,
	UTIL_PROPERTIES_UNSET_PROPERTY,

	/*-----------------------------------------------------------------------*/
	UTIL_SIMPSTRING_START_TAG = 1300,
	UTIL_SIMPSTRING_WRONG_FORMAT,
	UTIL_SIMPSTRING_OUT_OF_MEMORY,

	/*-----------------------------------------------------------------------*/
	UTIL_NETWORK_START_TAG = 1400,
	UTIL_NETWORK_TOOLONG_HOSTNAME,
	UTIL_NETWORK_FAIL_GET_NETCARD_CONFIG,
	UTIL_NETWORK_REMOTE_CLOSED,
	UTIL_NETWORK_FAIL_CREATESOCKET,
	UTIL_NETWORK_FAIL_GETHOST,
	UTIL_NETWORK_FAIL_BIND,
	UTIL_NETWORK_FAIL_CONNECT,
	UTIL_NETWORK_FAIL_RECV,
	UTIL_NETWORK_FAIL_SETFCNTL,

	ERROR_COUNT_MAX = 2000
};

struct ErrorDetailData {
	int				 Code;
	char	   		 Message[256];						/* Detail of message. */
};

typedef struct ErrorDetailData   ErrorDetailData;
typedef struct ErrorDetailData  *ErrorDetail;

#define ERRREPORTPOS __FILE__,__LINE__,__FUNCTION__
#define ERRORPOS_FORMAT "[%s:%d:%s]"

const char *getErrorCodeExplain(int errcode);

#define IS_TO_RM_RPC_ERROR(errcode) (((errcode)/100*100) == COMM2RM_CLIENT_START_TAG)

#endif /* RESOURCE_MANAGER_ERROR_CODE_H */
