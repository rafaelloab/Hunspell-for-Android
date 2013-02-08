#include "hunsjni.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <hunspell.hxx>
#include <string.h>

#define LOG_TAG "NDK" // text for log tag
#define LOG_DEBUG "NDK"

using namespace std;

Hunspell * pMS = NULL;

/*input: (String str1, String str2) - (path to dictionary, path to affix)
*return: int; unecessary?change to void?
*/
JNIEXPORT jint JNICALL Java_com_poc_spellchecker_Hunspell_init(JNIEnv* env,
		jobject thiz, jstring aff, jstring dic) {

	const char *nativeStringAff = env->GetStringUTFChars(aff, 0);
	const char *nativeStringDic = env->GetStringUTFChars(dic, 0);

	char* Affix = (char*) nativeStringAff;
	char* Dictionary = (char*) nativeStringDic;

	/// = /sdcard/dir/as.dic
	
	//Hunspell constructor
	pMS = new Hunspell(Affix, Dictionary);
	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, " Dicionário carregado.");


	return 0;
}

/*input: String - word to spellcheker
**return: String - results separated by space*/
JNIEXPORT jstring JNICALL Java_com_poc_spellchecker_Hunspell_test(JNIEnv* env,
		jobject thiz, jstring word) {

	const char *nativeStringWord = env->GetStringUTFChars(word, 0);
	char* Word = (char*) nativeStringWord;

	int k;
	int dp;
	char buf[101];

	FILE* wtclst;

	strcpy(buf, Word);

	
	k = strlen(buf);
	*(buf + k) = '\0';
	dp = pMS->spell(buf);
	//word spelled correct, returns "correct"
	if (dp) {

		
		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "\"%s\" is okay\n",
				buf);
		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "\n");

		return env->NewStringUTF("correct");

		//palavra errada, retorna possíveis correções
	} else {
		
		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,
				"\"%s\" is incorrect!\n", buf);

		char ** wlst;
		int ns = pMS->suggest(&wlst, buf);
		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "  %i suggestions:\n",
				ns);

		for (int i = 0; i < ns; i++) {
		
			__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "    ...\"%s\"\n",
					wlst[i]);

			

		}

		char *palavras = new char[ns * 10];

		memset(palavras, 0, ns * 10 * sizeof(char));

		for (int jj = 0; jj < ns; jj++) {

		//concatena the suggestions separated by space
			strcat(palavras, wlst[jj]);
			strcat(palavras, " ");
		}

		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s \n", palavras);

		pMS->free_list(&wlst, ns);
		

		return env->NewStringUTF(palavras);

	}
}

//destructor
JNIEXPORT void JNICALL Java_com_poc_spellchecker_Hunspell_finalize(JNIEnv* env,
		jobject thiz) {

	delete pMS;
	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "finalize");

}

/*input: String - word to be veryfied
 * return: int - returns zero if wrond or not in the dictionary, otherwise, returns different then zero
*/
JNIEXPORT jint JNICALL Java_com_poc_spellchecker_Hunspell_spell(JNIEnv* env,
		jobject thiz, jstring word) {

	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Hunspell_spell");

	const char *nativeStringWord = env->GetStringUTFChars(word, 0);
	char* Word = (char*) nativeStringWord;

	int k;
	int dp;
	char buf[101];

	strcpy(buf, Word);

	k = strlen(buf);
	*(buf + k) = '\0';
	dp = pMS->spell(buf);

	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "spell: %i", dp);


	return dp;
}


 
JNIEXPORT jobjectArray Java_com_poc_spellchecker_Hunspell_suggest
  (JNIEnv *env, jobject obj, jstring device){
 
        //transfer data from jstring to char*
        const char* cdevice = env->GetStringUTFChars(device,NULL);
       // string str; //create a string to handle
		
		char ** wlst;
 
      int ns = pMS->suggest(&wlst, cdevice);
	  
	  char *cRetorno[ns]; 
	  
	  for (int i = 0; i < ns; i++) {
		
			__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "    ...\"%s\"\n",
			
					wlst[i]);
					
					cRetorno[i] = wlst[i];

			

		}
	  

        //create return to java code
        jobjectArray retorno;
        //create an array jobjecArray with "i" elements.
        retorno = (jobjectArray)env->NewObjectArray(ns,env->FindClass("java/lang/String"),env->NewStringUTF(""));
 
        //add content of cRetorno inside retorno.
        for (int x = 0; x < ns; x++){
                env->SetObjectArrayElement(retorno,x,env->NewStringUTF(cRetorno[x]));
        }
		
		pMS->free_list(&wlst, ns);
 
        return retorno;
}

 

		

/*jstring JNU_NewStringNative(JNIEnv *env, const char *str)
 {
 jstring result;

 jbyteArray bytes = 0;

 int len;

 len = strlen(palavras);
 bytes = env->NewByteArray(env, len);

 env->SetByteArrayRegion(env, bytes, 0, len,
 (jbyte *)palavras);
 result = env->NewObject(env, Class_java_lang_String,
 MID_String_init, bytes);
 env->DeleteLocalRef(env, bytes);
 return result;


 }*/

/*

 Hunspell(const char * affpath, const char * dpath, const char * key = NULL);
 ~Hunspell();



 int add_dic(const char * dpath, const char * key = NULL);

 int spell(const char * word, int * info = NULL, char ** root = NULL);

 int suggest(char*** slst, const char * word);

 void free_list(char *** slst, int n);

 char * get_dic_encoding();


 int analyze(char*** slst, const char * word);

 int stem(char*** slst, const char * word);

 int stem(char*** slst, char ** morph, int n);

 int generate(char*** slst, const char * word, const char * word2);

 int generate(char*** slst, const char * word, char ** desc, int n);


 int add(const char * word);

 int add_with_affix(const char * word, const char * example);

 int remove(const char * word);



 const char * get_wordchars();

 unsigned short * get_wordchars_utf16(int * len);

 struct cs_info * get_csconv();
 
 const char * get_version(); */

