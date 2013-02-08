package com.poc.spellchecker;

public class Hunspell {
	
	static {
		System.loadLibrary("hunspell");
	}

	public native int init(String aff, String dic);

	public native String test(String word);

	public native void finalize();

	public native int spell(String word);

	public native String[] suggest(String word);

}
