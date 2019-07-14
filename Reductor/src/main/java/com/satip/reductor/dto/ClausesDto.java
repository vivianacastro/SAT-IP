package com.satip.reductor.dto;

import java.util.ArrayList;
import java.util.List;

public class ClausesDto {
	private List<Integer> values;

	public ClausesDto() {
		super();
		this.values = new ArrayList<>();
	}

	/**
	 * @return the values
	 */

	public List<Integer> getValues() {
		return values;
	}

	/**
	 * @param values the values to set
	 */
	public void setValues(List<Integer> values) {
		this.values = values;
	}

	public void addValue(Integer value) {
		this.values.add(value);
	}
}
