/* PostScript global procedure/function interfaces

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: January 20, 1983
Edit History:
Chuck Geschke: Sat Feb 18 10:46:02 1984
Tom Boynton: Thu Mar  3 15:50:50 1983
End Edit History.
*/

#ifndef PROCS_H
#define PROCS_H

#define _inline
#define _priv extern

/* multi-value function return types */

/*dict*/	typedef struct {boolean known; KVLoc kvloc} DictKVLoc;
/*dict*/	typedef struct {boolean known; Object value} DictVal;
/*scan*/	typedef struct {boolean found,error; Object token} TokenRet;

/* public procedures */

_array	Object		AGet(/*arrayObject,index:cardinal*/);
_vm	arrayObject	AllocArray(/*length:cardinal*/);
_vm	Offset		AllocCache();
_vm	dictObject	AllocDict(/*size:cardinal*/);
_vm	strObject	AllocString(/*length:cardinal*/);
_array			ALoad(/*Stack,arrayObject*/);
_stack	arrayObject	ArrayFromStack(/*arrayObject,Stack*/);
_array	procedure	ArrayInit(/*InitReason*/);
_inline	/*		Assert(boolean);*/
_error			AssertFailed();
_array			AStore(/*Stack,arrayObject*/);

_dict			Begin(/*dictObject*/);
_error			BUG(/*string*/);

_error			CantHappen();
_stack			ClearStack(/*Stack*/);
_stream			CloseFile(/*stmObject*/);
_type	Equality	Compare(/*a,b: Object*/);
_stack			Copy(/*Stack,cardinal*/);
_stack	cardinal	CountStack(/*Stack,max:cardinal*/);
_stack	cardinal	CountToMark(/*Stack*/);
_stream	stmObject	CreateFileStream(/*name:strObject,access:string*/);

_dict			Def(/*key,value:Object*/);
_dict	dictObject	Dict(/*maxlen:cardinal*/);
_dict	Object		DictGet(/*dictObject,Object*/);
_dict	procedure	DictInit(/*InitReason*/);
_dict	cardinal	DictLength(/*dictObject*/);
_dict			DictPut(/*dictObject,key,value:Object*/);
_cntrl			DisableCC();
_inline	/*		DoCmd(index:cardinal);*/

_cntrl			EnableCC();
_cntrl			EPRINT();
_type	boolean		Equal(/*a,b:Object*/);
_error			ERROR(/*nameObject*/);
_error	procedure	ErrorInit(/*InitReason*/);
_exec	procedure	ExecInit(/*InitReason*/);
_vm	charptr		EXPAND(/*charptr,count,size:integer*/);
_dict			ExpandCache(/*start,max:cardinal*/);

_dict	Object		ForceGet(/*dictObject,key:Object*/);
_dict			ForcePut(/*dictObject,key,value:Object*/);
_vm			FREE(/*charptr*/);

_exec	boolean		GetAbort();
_inline	/*boolean	GetCLine(StreamHandle);*/
_inline	/*boolean	GetILine(StreamHandle);*/
_scan	boolean		GetLine(/*StreamHandle,type*/);
_inline /*StreamHandle	GetStream(stmObject);*/
_inline /*charptr	GetVMFirst();*/
_inline /*charptr	GetVMLast();*/

_name	cardinal	HashString(/*strObject*/);

_dict			InitCache();
_inline	/*		InitRnum(integer);*/

_dict	boolean		Known(/*dictObject,nameObject/*);

_scan	boolean		LineComplete(/*string*/);
_cntrl	boolean		LineFEOF();
_cntrl	char		LineGETC();
_cntrl			LineUNGETC(/*char*/);

_name	nameObject	MakeName(/*string*/);
_stream	stmObject	MakeStream(/*StreamHandle,tag:cardinal*/);
_inline	/*strObject	MakeString(string);*/
_inline	/*strObject	MakeStringX(string);*/
_exec			MarkLoop();
_math	procedure	MathInit(/*InitReason*/);

_name	procedure	NameInit(/*InitReason*/);
_name	strObject	NameToString(/*nameObject*/);
_vm	charptr		NEW(/*count,size:integer*/);
_stack	Stack		NewStack(/*cardinal*/);

_stack			Overflow(/*Stack*/);

_type	Coord		PopCoord();
_cntrl			PRINT();
_cntrl			PRINTCHAR();
_stream			psEcho(/*boolean*/);
_dict	procedure	PSEnd();
_cntrl	procedure	PSQuit();
_exec	boolean		psExecute(/*Object*/);
_stack	Object		psPop(/*Stack*/);
_type	arrayObject	psPopArray(/*Stack*/);
_type	boolean		psPopBoolean(/*Stack*/);
_type	cardinal	psPopCardinal(/*Stack*/);
_type	cmdObject	psPopCommand(/*Stack*/);
_type	dictObject	psPopDict(/*Stack*/);
_type	integer		psPopInteger(/*Stack*/);
_type	real		psPopReal(/*Stack*/);
_type	strObject	psPopRString(/*Stack*/);
_type	stmObject	psPopStream(/*Stack*/);
_type	strObject	psPopString(/*Stack*/);
_stack			psPush(/*Stack,Object*/);
_type			psPushBoolean(/*Stack,boolean*/);
_type			psPushCardinal(/*Stack,cardinal*/);
_type			psPushInteger(/*Stack,integer*/);
_type			psPushReal(/*Stack,real*/);
_type			PushCoord(/*Coord*/);

_dict			ReadOnlyDict(/*dictObject*/);
_cntrl			RegisterExplicit(/*string,procedure*/);
_cntrl	cmdObject	RegisterInternal(/*string,procedure*/);
_vm			RegisterQuitProc(/*procedure*/);
_vm			RegisterRestoreProc(/*procedure*/);
_vm			RegisterSaveProc(/*procedure*/);
_exec			ResetExecStk(/*errorObject*/);
_scan			RestoreLine(/*LineRecord*/);
_stack			RestoreStack(/*Stack,mark:Node*/);
_inline	/*byte		Rnum8();*/

extern			SaveFC(/*StreamHandle*/); /*FontDisk*/
_scan	LineRecord	SaveLine();
_scan	procedure	ScannerInit(/*InitReason*/);
_dict	DictKVLoc	SearchDict(/*DictPtr,key:Object*/);
_exec			SetAbort(/*flag:boolean*/);
_stack	procedure	StackInit(/*InitReason*/);
_stream	procedure	StreamInit(/*InitReason*/);
_scan			StreamToken(/*stmObject,*TokenRet*/);
_string	integer		StringCompare(/*a,b:strObject*/);
_string	procedure	StringInit(/*InitReason*/);
_inline	/*		StringText(strObject,string);*/
_scan			StringToken(/*strObject,strObjPtr,*TokenRet*/);
_name	nameObject	StringToName(/*strObject*/);
_string	strObject	SubString(/*strObject,first,len:cardinal*/);
_inline	/*Object	SysDictGet(Object);*/

_stack	Object		Top(/*Stack*/);
_dict			TryToLoad(/*key:Object,*DictVal*/);
_type	procedure	TypeInit(/*InitReason*/);

_stack			Underflow(/*Stack*/);
_exec			UnmarkLoop();
_cntrl	procedure	Unregistered();

_inline /*ObjPtr	VMArrayPtr(arrayObject);*/		
_vm			VMCopyArray(/*src,dst:arrayObject*/);
_vm	dictObject	VMAllocCopyDict(/*dictObject*/);
_vm	dictObject	VMCopyDict(/*from,to:dictObject*/);
_vm			VMCopyString(/*src,dst:strObject*/);
_vm			VMFinish();
_vm			VMFlush();
_inline	/*		VMGetCE(CEptr,cardinal);*/
_inline	/*CEptr		VMGetCEPtr(cardinal);*/
_inline	/*character	VMGetChar(strObject,cardinal);*/
_inline	/*		VMGetDict(DictPtr,dictObject);*/
_inline	/*DictPtr	VMGetDictPtr(dictObject);*/
_inline	/*Object	VMGetElem(arrayObject,cardinal);*/
_inline	/*		VMGetKeyVal(KeyValPtr,KeyValOffset);*/
_inline	/*KeyValPtr	VMGetKeyValPtr(KeyValOffset);*/
_vm			VMGetRoot(/*Root*/);
_vm			VMGetText(/*strObject,string*/);
_inline	/*		VMGetValue(ObjPtr,KeyValOffset);*/
_vm	procedure	VMInit(/*InitReason*/);
_vm			VMPutCE(/*cardinal,CEptr*/);
_vm			VMPutChar(/*strObject,cardinal,character*/);
_vm			VMPutDict(/*dictObject,DictPtr*/);
_vm			VMPutElem(/*arrayObject,cardinal,Object*/);
_vm			VMPutKeyVal(/*KeyValOffset,KeyValPtr*/);
_vm			VMPutRoot(/*Root*/);
_vm			VMPutText(/*strObject,string*/);
_vm			VMPutValue(/*KeyValOffset,ObjPtr*/);
_vm	boolean		VMStart();
_inline /*string	VMStrPtr(strObject);*/		

_vm	integer		WordsForChars(/*integer*/);

/* definition of inlines procedures */

#define Assert(c)\
	if (!(c)) {AssertFailed();}

#define DoCmd(i)\
	if(i<commands.length){commands.cmds[i].proc();}else{Unregistered();}

#define GetCLine(s) GetLine(s,1)

#define GetILine(s) GetLine(s,0)

#define GetStream(s)\
	(((s.val.stmval == root->stamp)&&(streams[s.length]!=NIL))?\
		streams[s.length]:(StreamHandle)(ERROR(invalidfile)))

#define GetVMFirst()\
	(&vm[vmInfo->free])

#define GetVMLast()\
	(&vm[vmInfo->last])

#define InitRnum(s) {rndnum = s;}

#define MakeString(s)\
	makestring((s),Lobj)

#define MakeStringX(s)\
	makestring((s),Xobj)

#define RnumSeed 11465

#define Rnum8()\
	((rndnum = (((rndnum*1069928045)+226908351)>>8) & 0x3fffffff) & 0377)

#define StringText(s,t)\
	VMGetText((s),(t))

#define SysDictGet(k)\
	DictGet(root->sysDict,k)

#define VMArrayPtr(ao)\
	(ObjPtr)(&vm[(ao).val.arrayval])

#define VMGetCE(cep,i)\
	*(cep) = *((CEptr)(&vm[root->cacheOffset+(i)*sizeof(CacheEntry)]))

#define VMGetCEPtr(i)\
	((CEptr)(&vm[root->cacheOffset+(i)*sizeof(CacheEntry)]))

#define VMGetChar(so,i)\
	(((string)(&vm[(so).val.strval]))[i])

#define VMGetDict(dp,d)\
	*(dp) = *((DictPtr)(&vm[(d).val.dictval]))

#define VMGetDictPtr(d)\
	((DictPtr)(&vm[(d).val.dictval]))

#define VMGetElem(ao,i)\
	(((ObjPtr)(&vm[(ao).val.arrayval]))[i])

#define VMGetKeyVal(kvp,kvo)\
	*(kvp) = *((KeyValPtr)(&vm[(kvo)]))

#define VMGetKeyValPtr(kvo)\
	((KeyValPtr)(&vm[(kvo)]))

#define VMGetValue(obp,kvo)\
	*(obp) = ((KeyValPtr)(&vm[(kvo)]))->value

#define VMStrPtr(so)\
	(string)(&vm[(so).val.strval])

/* the following procedures are "externed" for access via macros */

_priv	strObject	makestring(/*string,obtype:cardinal*/);

#endif  PROCS_H
