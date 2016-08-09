#ifndef _CJSON_DIRECT_H_
#define _CJSON_DIRECT_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
	KEY_TYPE_NULL,
	KEY_TYPE_U8,
	KEY_TYPE_U16,
	KEY_TYPE_U32,
	KEY_TYPE_FLOAT,
	KEY_TYPE_DOUBLE,
	KEY_TYPE_STRING,	//����洢���ַ���

	/**
	 * �ⲿ�洢���ַ�����
	 * ����cjson_string2object�γɵĽṹ�壬
	 * ��Ҫ����cjson_string2object_array_free �ͷ���Դ
	 *
	 */
	KEY_TYPE_STRING_PTR,	//

	KEY_TYPE_OBJECT,

	/**
		 ע�⣬������ַ������飬ֻ֧�ֶ�ά���飬���Ҳ�֧���ⲿ�ַ���
		 char stringArray[2][3];// ������֧�ֵ�
		 char *stringArray[2];//�����ǲ�֧�ֵġ���Ϊ�����ⲿ�ַ���
	*/
	KEY_TYPE_ARRAY,
	KEY_TYPE_MAX
}key_type_e;


typedef struct key_info_s{
	int csize;					///< ���ṹ���С
	key_type_e type;				///< ��Ա����
	char *key;					///< ��Ա����
	int offset;					///< ��Աƫ�Ƶ�ַ
	int ksize;						///< ��Ա��С
	struct key_info_s *sub_key;	///< ����#KEY_TYPE_OBJECT���ͣ���������Ͷ���
	int arraycnt;					///< ����#KEY_TYPE_ARRAY���ͣ������
	key_type_e arraytype;		///< ����#KEY_TYPE_ARRAY���ͣ����Ա������
}json_kinfo_t;


/*��Ա�ڽṹ���е�ƫ�Ƶ�ַ*/
#define NAME_OFFSET(type,name) ((int)(&(((type *)0)->name)))
#define NAME_SIZE(type,name) (sizeof((((type *)0)->name)))


/**
 *@brief �γɽṹ��
 *@param ctype �ṹ������
 *@param ktype ��Ա����
 *@param kname ��Ա��
 *@param subkey ���keytypeΪ#KEY_TYPE_OBJECT����Ϊ���Ӧ�ṹ���#json_kinfo_t ָ��
 *@param arraycnt ����#KEY_TYPE_ARRAY���ͣ������
 *@param arraytype ����#KEY_TYPE_ARRAY���ͣ����Ա������
 *
 */
#define MAKE_ARRAY_INFO(ctype, ktype, kname, subkey, arraycnt, arraytype) {sizeof(ctype), ktype, #kname, NAME_OFFSET(ctype,kname), NAME_SIZE(ctype,kname), subkey, arraycnt, arraytype}

#define MAKE_KEY_INFO(ctype, ktype, kname, subkey) MAKE_ARRAY_INFO(ctype, ktype, kname, subkey, 0, KEY_TYPE_NULL)
#define MAKE_END_INFO()	{0, KEY_TYPE_NULL, NULL, 0, 0, NULL}


/**
 *@brief ��JSON��ʽ���ַ���ת��Ϊ�ṹ��
 *@param kinfo ���� �ṹ����Ϣ������ʶ��ṹ�����Ա
 *@param string ���� Ҫת�����ַ���
 *@param obj ��� �ṹ��ָ�롣���ΪNULL�����Զ������ڴ棬��Ҫ�ͷ�
 *
 *@return obj��ʧ��ʱ����NULL
 *
 */
void *cjson_string2object(json_kinfo_t *kinfo, char *string, void *obj);

/**
 *@brief ��JSON��ʽ���ַ���ת��Ϊ�ṹ������
 *@param kinfo ���� �ṹ����Ϣ������ʶ��ṹ�����Ա
 *@param string ���� Ҫת�����ַ���
 *@param obj_array ��� �ṹ�������ָ�롣���ΪNULL�����Զ������ڴ棬��Ҫ�ͷ�
 *@param maxCnt ���� �����װ�ص�obj�ĸ���
 *@param cnt ��� ʵ�ʵõ���obj�ĸ���
 *
 *@return obj��ʧ��ʱ����NULL
 *
 */
void *cjson_string2object_array(json_kinfo_t *kinfo, char *string, void *obj_array, int maxCnt, int *cnt);

/**
 *@brief ��#cjson_string2object �� #cjson_string2object_array ת����Ľṹ�壬����Ҫ���ͷŲ�����
 *@param kinfo ���� �ṹ����Ϣ������ʶ��ṹ�����Ա
 *@param obj_array ��Ҫ�ͷŵ���Դ
 *@param cnt ���� ʵ�ʵõ���obj�ĸ��������� #cjson_string2object ��cntΪ1
 *
 */
void cjson_string2object_array_free(json_kinfo_t *kinfo, void *obj_array, int cnt);

/**
 *@brief ���ṹ��ת��Ϊ�ַ���
 *@param kinfo ���� �ṹ����Ϣ������ʶ��ṹ�����Ա
 *@param obj ���� Ҫת���Ľṹ���ַ
 *
 *@return json��ʽ���ַ�����NULL if failed
 *
 *@note �䷵�ص��ַ�������Ҫ��free�ͷ�
 *
 */
char *cjson_object2string(json_kinfo_t *kinfo, void *obj);

/**
 *@brief ���ṹ������ת��ΪJSON�ַ���
 *@param kinfo ���� �ṹ����Ϣ������ʶ��ṹ�����Ա
 *@param obj_array ���� Ҫת���Ľṹ�������ָ��
 *@param cnt �������
 *
 *@return json��ʽ���ַ�����NULL if failed
 *
 *@note �䷵�ص��ַ�������Ҫ��free�ͷ�
 *
 */
char *cjson_object_array2string(json_kinfo_t *kinfo, void *obj_array, int cnt);


#ifdef __cplusplus
}
#endif

#endif

