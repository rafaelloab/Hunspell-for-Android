package com.poc.spellchecker;

import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import android.app.Activity;
import android.app.ProgressDialog;
import android.os.AsyncTask;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

/**
 * @author rafaello
 * 
 *         This project intends to demonstrate the use of the Hunspell C code
 *         compiled for Android, feel free to use it as you want. This class is
 *         the main and only Activity of this project, it consists in loading
 *         the Hunspell native code passing the path to the dictionary and the
 *         affix files, this happens in a AsyncTask which is a thread. After
 *         loading Hunspell, words may be typed in the editText box, a TextWatch
 *         listener will be "watching" every key stroke in the editText and then
 *         automatically starts another AsyncTask thread to do the
 *         spellchecking, which will return the result above the editText, will
 *         show words if it is misspelled, otherwise nothing will be shown.
 * 
 * 
 *         Change the dictionary for the language you want to spellcheck.
 *         http://hunspell.sourceforge.net/ points to the source of many
 *         different dictionaries languages.
 * 
 *         REMEMBER TO PUT THE *.DIC and *.AFF FILES IN THE CORRECT FOLDERS IN
 *         THE EXTERNAL MEMORY OF YOUR DEVICE
 * 
 *         e.g: "/sdcard/HunspellDictionary/en_US.dic"
 *         "/sdcard/HunspellDictionary/en_US.aff"
 */
public class SpellCheckerActivity extends Activity {

	public Hunspell mHunspell;
	public EditText text;
	public TextView suggestionTextView;
	private long initialTime = 0;
	private long elapsedTime;
	public static final String TAG = "SpellChecker";
	public ProgressDialog splash;

	
	//not the best way to refer to those files, should use getExternalStorageDirectory or something
	public String dicPath = "/sdcard/HunspellDictionary/en_US.dic";
	public String affPath = "/sdcard/HunspellDictionary/en_US.aff";

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.main);

		suggestionTextView = (TextView) findViewById(R.id.suggestion);
		suggestionTextView.setHeight(10);

		text = (EditText) findViewById(R.id.editText1);

		text.addTextChangedListener(change);

		new InitializeHunspell().execute();
		splash = ProgressDialog.show(this, "", "Loading...");
		// Sets the action bar navigation mode

	}

	/**
	 * A listener for a new letter typed. Every new letter typed it
	 * automatically starts a spellchecker for the current word in the editText
	 * box.
	 */
	private TextWatcher change = new TextWatcher() {

		public void onTextChanged(CharSequence s, int start, int before,
				int count) {

			new Suggest().execute(s.toString());
			Log.d(TAG, "onTextChanged: " + s);

		}

		public void afterTextChanged(Editable s) {
			// TODO Auto-generated method stub

		}

		public void beforeTextChanged(CharSequence s, int start, int count,
				int after) {
			// TODO Auto-generated method stub

		}

	};

	/**
	 * @author rafaello
	 * 
	 *         This is where the Hunspell native aplication is loaded. I know,
	 *         this class should be in another file.
	 * 
	 */
	private class InitializeHunspell extends AsyncTask<Void, Void, Void> {

		@Override
		protected Void doInBackground(Void... params) {
			Log.d(TAG, "doInBackground");

			initialTime = System.currentTimeMillis();
			mHunspell = new Hunspell();

			// the *.aff and *.dic files paths are passed to load Hunspell
			mHunspell.init(affPath, dicPath);
			return null;
		}

		// when finiss loading, dismiss the progress dialog
		@Override
		protected void onPostExecute(Void result) {
			Log.d(TAG, "" + result);

			elapsedTime = System.currentTimeMillis() - initialTime;
			Log.d(TAG, "elapsed time:  " + elapsedTime + "ms");
			splash.cancel();
			super.onPostExecute(result);

		}

	}

	/**
	 * @author rafaello
	 * 
	 *         This class starts a new thread to do the spellchecking.
	 */
	private class Suggest extends AsyncTask<String, Void, String[]> {

		private String[] suggestionsArray;

		@Override
		protected String[] doInBackground(String... params) {
			Log.d(TAG, "" + params[0]);

			if (mHunspell.spell(params[0]) == 0) {
				Log.d(TAG, "words not found: ");
				suggestionsArray = mHunspell.suggest(params[0]);
				return suggestionsArray;

			}

			return null;
		}

		@Override
		protected void onPostExecute(String[] result) {

			if (result != (null)) {
				suggestionTextView.setText("");
				for (int i = 0; i < result.length; i++) {

					suggestionTextView.setText(suggestionTextView.getText()
							+ "\n" + result[i]);

				}
				super.onPostExecute(result);
			} else {
				suggestionTextView.setText("");
			}
		}

	}

	public static String fileToString(String file) {
		String result = null;
		DataInputStream in = null;

		try {
			File f = new File(file);
			byte[] buffer = new byte[(int) f.length()];
			in = new DataInputStream(new FileInputStream(f));
			in.readFully(buffer);
			result = new String(buffer);
		} catch (IOException e) {
			throw new RuntimeException("IO problem in fileToString", e);
		} finally {
			try {
				in.close();
			} catch (IOException e) { /* ignore it */
			}
		}

		return result;
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();

		Log.d(TAG, "onDestroy");
		// mHunspell.finalize();

	};

}
