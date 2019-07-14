package com.satip.reductor.dto;

import java.util.ArrayList;
import java.util.List;

public class DimacsDto {
	private List<String> comments;
	private String format;
	private int variables;
	private int clauses;
	private List<ClausesDto> clausesDto;

	public DimacsDto() {
		super();
		this.comments = new ArrayList<>();
		this.clausesDto = new ArrayList<>();
	}

	/**
	 * @return the comments
	 */
	public List<String> getComments() {
		return comments;
	}

	/**
	 * @param comments the comments to set
	 */
	public void setComments(List<String> comments) {
		this.comments = comments;
	}

	public void addComment(String comment) {
		this.comments.add(comment);
	}

	/**
	 * @return the format
	 */
	public String getFormat() {
		return format;
	}

	/**
	 * @param format the format to set
	 */
	public void setFormat(String format) {
		this.format = format;
	}

	/**
	 * @return the variables
	 */
	public int getVariables() {
		return variables;
	}

	/**
	 * @param variables the variables to set
	 */
	public void setVariables(int variables) {
		this.variables = variables;
	}

	/**
	 * @return the clauses
	 */
	public int getClauses() {
		return clauses;
	}

	/**
	 * @param clauses the clauses to set
	 */
	public void setClauses(int clauses) {
		this.clauses = clauses;
	}

	/**
	 * @return the clausesDto
	 */
	public List<ClausesDto> getClausesDto() {
		return clausesDto;
	}

	/**
	 * @param clausesDto the clausesDto to set
	 */
	public void setClausesDto(List<ClausesDto> clausesDto) {
		this.clausesDto = clausesDto;
	}
	public void addClausesDto(ClausesDto clausesDto) {
		this.clausesDto.add(clausesDto);
	}
}
