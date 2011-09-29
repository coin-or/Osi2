# Experimental makefile for LaTeX documents. It's not foolproof as far as
# resolving references and citations, but it does decently well. Because
# the .aux file is in the dependency tree, we'll always run tex, even if
# nothing's changed, because the .aux file is newer than the .tex or .bbl
# files.

BASE = osi2design

INKPICS = apihierarchy.eps

ISLDIR = .

# The real targets

$(BASE): $(BASE).dvi
$(BASE)_slides: $(BASE)_slides.dvi

# Generic rules


%.eps:	%.epsu
	@echo "munching $< to produce $@"
	@sed -n -f $(ISLDIR)/epsupatch.sed < $< > $@

%.eps:	%.epsx
	@echo "munching $< to produce $@"
	@sed -n -f $(ISLDIR)/epsupatch.sed < $< | \
	 sed -f $(ISLDIR)/epsxpatch.sed > $@

%.pdf:	%.eps
	@echo "munching $< to produce $@"
	@$(ISLDIR)/adjustbb -1 -1 +1 +1 $<
	@epstopdf --outfile=$@ $<

%.dvi:	%.tex
	@echo "TeXing $< to produce $@ ..."
	@latex $<
	@echo "done."

%.bbl:	%.aux
	@echo "BibTeXing $< to produce $@ ..."
	@bibtex $* 
	@echo "done."

%.aux:	%.tex
	@echo "TeXing $< to produce $@ ..."
	@latex $<
	@echo "done."


$(BASE).dvi: $(BASE).tex \
	  $(INKPICS) \
	  $(ISLPICS:.epsu=.eps) $(ISLXPICS:.epsx=.eps)

$(BASE).pdf: $(BASE).tex \
	  $(INKPICS:.eps=.pdf) \
	  $(ISLPICS:.epsu=.pdf) $(ISLXPICS:.epsx=.pdf)
	@echo "TeXing $< to produce $@ ..."
	@pdflatex $<
	@echo "done."

$(BASE)_slides.dvi: $(BASE)_slides.tex \
	  $(ISLPICS:.epsu=.eps) $(ISLXPICS:.epsx=.eps)

