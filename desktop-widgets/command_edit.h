// SPDX-License-Identifier: GPL-2.0
// Note: this header file is used by the undo-machinery and should not be included elsewhere.

#ifndef COMMAND_EDIT_H
#define COMMAND_EDIT_H

#include "command_base.h"
#include "core/subsurface-qt/DiveListNotifier.h"

#include <QVector>

// These are commands that edit individual fields of a set of dives.
// The implementation is very OO-style. Out-of-fashion and certainly
// not elegant, but in line with Qt's OO-based design.
// The actual code is in a common base class "Command::EditBase". To
// read and set the fields, the base class calls virtual functions of
// the derived classes.
//
// To deal with different data types, the base class is implemented
// as a template. The template parameter is the type to be read or
// set. Thus, switch-cascades and union trickery can be avoided.

// We put everything in a namespace, so that we can shorten names without polluting the global namespace
namespace Command {

template <typename T>
class EditBase : public Base {
protected:
	T value; // Value to be set
	T old; // Previous value

	void undo() override;
	void redo() override;
	bool workToBeDone() override;

	// Dives to be edited. For historical reasons, the *last* entry was
	// the active dive when the user initialized the action. This dive
	// will be made the current dive on redo / undo.
	std::vector<dive *> dives;
public:
	EditBase(const QVector<dive *> &dives, T newValue, T oldValue);

protected:
	// Get and set functions to be overriden by sub-classes.
	virtual void set(struct dive *d, T) const = 0;
	virtual T data(struct dive *d) const = 0;
	virtual QString fieldName() const = 0;	// Name of the field, used to create the undo menu-entry
	virtual DiveField fieldId() const = 0;
};

class EditNotes : public EditBase<QString> {
public:
	using EditBase<QString>::EditBase;	// Use constructor of base class.
	void set(struct dive *d, QString s) const override;
	QString data(struct dive *d) const override;
	QString fieldName() const override;
	DiveField fieldId() const override;
};

class EditSuit : public EditBase<QString> {
public:
	using EditBase<QString>::EditBase;	// Use constructor of base class.
	void set(struct dive *d, QString s) const override;
	QString data(struct dive *d) const override;
	QString fieldName() const override;
	DiveField fieldId() const override;
};

class EditRating : public EditBase<int> {
public:
	using EditBase<int>::EditBase;	// Use constructor of base class.
	void set(struct dive *d, int value) const override;
	int data(struct dive *d) const override;
	QString fieldName() const override;
	DiveField fieldId() const override;
};

class EditVisibility : public EditBase<int> {
public:
	using EditBase<int>::EditBase;	// Use constructor of base class.
	void set(struct dive *d, int value) const override;
	int data(struct dive *d) const override;
	QString fieldName() const override;
	DiveField fieldId() const override;
};

class EditAirTemp : public EditBase<int> {
public:
	using EditBase<int>::EditBase;	// Use constructor of base class.
	void set(struct dive *d, int value) const override;
	int data(struct dive *d) const override;
	QString fieldName() const override;
	DiveField fieldId() const override;
};

class EditWaterTemp : public EditBase<int> {
public:
	using EditBase<int>::EditBase;	// Use constructor of base class.
	void set(struct dive *d, int value) const override;
	int data(struct dive *d) const override;
	QString fieldName() const override;
	DiveField fieldId() const override;
};

class EditDiveSite : public EditBase<struct dive_site *> {
public:
	using EditBase<struct dive_site *>::EditBase;	// Use constructor of base class.
	void set(struct dive *d, struct dive_site *value) const override;
	struct dive_site *data(struct dive *d) const override;
	QString fieldName() const override;
	DiveField fieldId() const override;

	// We specialize these so that we can send dive-site changed signals.
	void undo() override;
	void redo() override;
};

// Edit dive site, but add a new dive site first. Reuses the code of EditDiveSite by
// deriving from it and hooks into undo() and redo() to add / remove the dive site.
class EditDiveSiteNew : public EditDiveSite {
public:
	OwningDiveSitePtr diveSiteToAdd;
	struct dive_site *diveSiteToRemove;
	EditDiveSiteNew(const QVector<dive *> &dives, const QString &newName, struct dive_site *oldValue);
	void undo() override;
	void redo() override;
};

class EditMode : public EditBase<int> {
	int index;
public:
	EditMode(const QVector<dive *> &dives, int indexIn, int newValue, int oldValue);
	void set(struct dive *d, int i) const override;
	int data(struct dive *d) const override;
	QString fieldName() const override;
	DiveField fieldId() const override;
};

// Fields that work with tag-lists (tags, buddies, divemasters) work differently and therefore
// have their own base class. In this case, it's not a template, as all these lists are base
// on strings.
class EditTagsBase : public Base {
	bool workToBeDone() override;

	// Dives to be edited. For historical reasons, the *last* entry was
	// the active dive when the user initialized the action. This dive
	// will be made the current dive on redo / undo.
	std::vector<dive *> dives;
	QStringList newList;	// Temporary until initialized
	struct dive *oldDive;	// Temporary until initialized
public:
	EditTagsBase(const QVector<dive *> &dives, const QStringList &newList, struct dive *d);

protected:
	QStringList tagsToAdd;
	QStringList tagsToRemove;
	void undo() override;
	void redo() override;

	// Getters, setters and parsers to be overriden by sub-classes.
	virtual QStringList data(struct dive *d) const = 0;
	virtual void set(struct dive *d, const QStringList &v) const = 0;
	virtual QString fieldName() const = 0;	// Name of the field, used to create the undo menu-entry
	virtual DiveField fieldId() const = 0;
};

class EditTags : public EditTagsBase {
public:
	using EditTagsBase::EditTagsBase;	// Use constructor of base class.
	QStringList data(struct dive *d) const override;
	void set(struct dive *d, const QStringList &v) const override;
	QString fieldName() const override;
	DiveField fieldId() const override;
};

class EditBuddies : public EditTagsBase {
public:
	using EditTagsBase::EditTagsBase;	// Use constructor of base class.
	QStringList data(struct dive *d) const override;
	void set(struct dive *d, const QStringList &v) const override;
	QString fieldName() const override;
	DiveField fieldId() const override;
};

} // namespace Command

#endif
