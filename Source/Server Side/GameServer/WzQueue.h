#ifndef __WZQUEUE_H__
#define __WZQUEUE_H__
#include <winsock2.h>

/*
#define MAX_BUFF_SIZE		8192

typedef struct _PER_IO_CONTEXT_L
{
	OVERLAPPED					Overlapped;
	WSABUF						wsabuf;
	CHAR						Buffer[MAX_BUFF_SIZE];
	int							nTotalBytes;
	int							nSentBytes;
	int							IOOperation;
	void*						ListNode;
} PER_IO_CONTEXT_L, * LPPER_IO_CONTEXT_L;
*/
typedef struct _ListNode
{
	struct _ListNode*	pUpLink;
	struct _ListNode*	pDownLink;
	unsigned char *		pObject;
	unsigned int		nSize;
	BOOL				bSending;
	int					nOfs;
	BYTE				headcode;
	int					uindex;
	//PER_IO_CONTEXT_L	IoCtxt;
} ListNode;

#define MAX_NODE		1280
#define SPIN_COUNT		4000

class  WZQueue
{
public:
	WZQueue(unsigned int nMaxNode = MAX_NODE);
	~WZQueue();

	unsigned int	GetCount();
	BOOL			IsEmpty();
	BOOL			AddToQueue(const BYTE* pObject, unsigned int nSize, BYTE headcode, int uindex);
	ListNode*		AddToQueueList(const BYTE* pObject, unsigned int nSize, BYTE headcode, int uindex);
	BOOL			GetFromQueue(BYTE* pObject, unsigned int * pSize, BYTE * headcode, int * uindex);
	//BOOL			GetFromQueue(BYTE* pObject, unsigned int * pSize);
	void			Init();
	void			Free();
	
	BOOL        StartQ();
	BOOL		NextQ();
	ListNode*	GetCurData(BYTE* pObject, unsigned int * pSize, BYTE * headcode, int * uindex);
	BOOL		Pop(ListNode* pCur, BYTE *pObject, int nOfs, int *nSize, int *sentbytes);
	void		DeleteNode(ListNode* pCur);
	void		SetCurDataSending(ListNode* pNode);


private:
	
	ListNode*	GetHeadNode();
    BOOL		AddTail(ListNode* pNewNode);
	void		DeleteAllNodes();

	

	unsigned int		m_Count;
	unsigned int		m_Max_Node;
    ListNode*		m_pHead;
	ListNode*		m_pTail;
	ListNode*		m_pCur;

	CRITICAL_SECTION	m_CriticalSection;
};

#endif
