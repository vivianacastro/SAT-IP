package com.satip.reductor.utils;

import java.util.ArrayList;

public class StringUtils {

	public static String[] trim(String[] parts) {
		ArrayList<String> result = new ArrayList<>();

		for (String part : parts) {
			if (part != null && !part.isEmpty() && !Constants.SPACE.equals(part)) {
				result.add(part);
			}
		}

		return result.toArray(new String[] {});
	}

}
