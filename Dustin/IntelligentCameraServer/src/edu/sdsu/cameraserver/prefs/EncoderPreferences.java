package edu.sdsu.cameraserver.prefs;

import edu.sdsu.cameraserver.R;
import android.os.Bundle;
import android.preference.PreferenceActivity;

public class EncoderPreferences extends PreferenceActivity{

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		addPreferencesFromResource(R.xml.prefs);
	}
	
}
