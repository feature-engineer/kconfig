/*
    This file is part of KDE.

    SPDX-FileCopyrightText: 2001, 2002, 2003 Cornelius Schumacher <schumacher@kde.org>
    SPDX-FileCopyrightText: 2003 Waldo Bastian <bastian@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCORECONFIGSKELETON_H
#define KCORECONFIGSKELETON_H

#include <kconfigcore_export.h>

#include <kconfiggroup.h>
#include <ksharedconfig.h>

#include <QDate>
#include <QHash>
#include <QRect>
#include <QStringList>
#include <QUrl>
#include <QVariant>

class KCoreConfigSkeletonPrivate;

class KConfigSkeletonItemPrivate;
/**
 * \class KConfigSkeletonItem kcoreconfigskeleton.h <KCoreConfigSkeleton>
 *
 * @short Class for storing a preferences setting
 * @author Cornelius Schumacher
 * @see KCoreConfigSkeleton
 *
 * This class represents one preferences setting as used by @ref KCoreConfigSkeleton.
 * Subclasses of KConfigSkeletonItem implement storage functions for a certain type of
 * setting. Normally you don't have to use this class directly. Use the special
 * addItem() functions of KCoreConfigSkeleton instead. If you subclass this class you will
 * have to register instances with the function KCoreConfigSkeleton::addItem().
 */
class KCONFIGCORE_EXPORT KConfigSkeletonItem
{
    Q_DECLARE_PRIVATE(KConfigSkeletonItem)
public:
    typedef QList<KConfigSkeletonItem *> List;
    typedef QHash<QString, KConfigSkeletonItem *> Dict;
    typedef QHash<QString, KConfigSkeletonItem *>::Iterator DictIterator;

    /**
     * Constructor.
     *
     * @param _group Config file group.
     * @param _key Config file key.
     */
    KConfigSkeletonItem(const QString &_group, const QString &_key);

    /**
     * Destructor.
     */
    virtual ~KConfigSkeletonItem();

    /**
     * Set config file group.
     */
    void setGroup(const QString &_group);

    /**
     * Return name of config file group.
     */
    QString group() const;

    /**
     * Set config file group but giving the KConfigGroup.
     * Allow the item to be in nested groups.
     * @since 5.68
     */
    void setGroup(const KConfigGroup &cg);

    /**
     * Return a KConfigGroup, the one provided by setGroup(const KConfigGroup&) if it's valid,
     * or make one from @p config and item's group.
     * @see setGroup(const QString &_group)
     * @see setGroup(KConfigGroup cg)
     * @since 5.68
     */
    KConfigGroup configGroup(KConfig *config) const;

    /**
     * Set config file key.
     */
    void setKey(const QString &_key);

    /**
     * Return config file key.
     */
    QString key() const;

    /**
     * Set internal name of entry.
     */
    void setName(const QString &_name);

    /**
     * Return internal name of entry.
     */
    QString name() const;

    /**
     * Set label providing a translated one-line description of the item.
     */
    void setLabel(const QString &l);

    /**
     * Return the label of the item.
     * @see setLabel()
     */
    QString label() const;

    /**
     * Set ToolTip description of item.
     * @since 4.2
     */
    void setToolTip(const QString &t);

    /**
     * Return ToolTip description of item.
     * @see setToolTip()
     * @since 4.2
     */
    QString toolTip() const;

    /**
     * Set WhatsThis description of item.
     */
    void setWhatsThis(const QString &w);

    /**
     * Return WhatsThis description of item.
     * @see setWhatsThis()
     */
    QString whatsThis() const;

    /**
     * The write flags to be used when writing configuration.
     * @since 5.58
     */
    void setWriteFlags(KConfigBase::WriteConfigFlags flags);

    /**
     * Return write flags to be used when writing configuration.
     * They should be passed to every call of KConfigGroup::writeEntry() and KConfigGroup::revertToDefault().
     * @since 5.58
     */
    KConfigBase::WriteConfigFlags writeFlags() const;

    /**
     * This function is called by @ref KCoreConfigSkeleton to read the value for this setting
     * from a config file.
     */
    virtual void readConfig(KConfig *) = 0;

    /**
     * This function is called by @ref KCoreConfigSkeleton to write the value of this setting
     * to a config file.
     * Make sure to pass writeFlags() to every call of KConfigGroup::writeEntry() and KConfigGroup::revertToDefault().
     */
    virtual void writeConfig(KConfig *) = 0;

    /**
     * Read global default value.
     */
    virtual void readDefault(KConfig *) = 0;

    /**
     * Set item to @p p
     */
    virtual void setProperty(const QVariant &p) = 0;

    /**
     * Check whether the item is equal to @p p.
     *
     * Use this function to compare items that use custom types,
     * because QVariant::operator== will not work for those.
     *
     * @param p QVariant to compare to
     * @return @c true if the item is equal to @p p, @c false otherwise
     */
    virtual bool isEqual(const QVariant &p) const = 0;

    /**
     * Return item as property
     */
    virtual QVariant property() const = 0;

    /**
     * Return minimum value of item or invalid if not specified
     */
    virtual QVariant minValue() const;

    /**
     * Return maximum value of item or invalid if not specified
     */
    virtual QVariant maxValue() const;

    /**
     * Sets the current value to the default value.
     */
    virtual void setDefault() = 0;

    /**
     * Exchanges the current value with the default value
     * Used by KCoreConfigSkeleton::useDefaults(bool);
     */
    virtual void swapDefault() = 0;

    /**
     * Return if the entry can be modified.
     */
    bool isImmutable() const;

    /**
     * Indicates if the item is set to its default value.
     *
     * @since 5.64
     */
    bool isDefault() const;

    /**
     * Indicates if the item has a different value than the
     * previously loaded value.
     *
     * @since 5.64
     */
    bool isSaveNeeded() const;

    /**
     * Returns the default value
     * @since 5.74
     */
    QVariant getDefault() const;

protected:
    explicit KConfigSkeletonItem(KConfigSkeletonItemPrivate &dd, const QString &_group, const QString &_key);

    /**
     * Sets mIsImmutable to @c true if mKey in config is immutable.
     * @param group KConfigGroup to check if mKey is immutable in
     */
    void readImmutability(const KConfigGroup &group);

    QString mGroup; ///< The group name for this item
    QString mKey; ///< The config key for this item
    QString mName; ///< The name of this item

    // HACK: Necessary to avoid introducing new virtuals in KConfigSkeletonItem
    // KF6: Use proper pure virtuals in KConfigSkeletonItem
    void setIsDefaultImpl(const std::function<bool()> &impl);
    void setIsSaveNeededImpl(const std::function<bool()> &impl);
    void setGetDefaultImpl(const std::function<QVariant()> &impl);

    KConfigSkeletonItemPrivate *const d_ptr;
};

class KPropertySkeletonItemPrivate;

/**
 * \class KPropertySkeletonItem kcoreconfigskeleton.h <KCoreConfigSkeleton>
 *
 * @short Class for proxying a QObject property as a preferences setting
 * @author Kevin Ottens
 * @see KConfigSkeletonItem
 *
 * This class represents one preferences setting as used by @ref KCoreConfigSkeleton.
 * Unlike other @ref KConfigSkeletonItem subclasses, this one won't store the preference
 * in KConfig but will use a QObject property as storage.
 * You will have to register instances of this class with the function KCoreConfigSkeleton::addItem().
 *
 * @since 5.65
 */
class KCONFIGCORE_EXPORT KPropertySkeletonItem : public KConfigSkeletonItem
{
    Q_DECLARE_PRIVATE(KPropertySkeletonItem)
public:
    /**
     * Constructor
     *
     * @param object The QObject instance which we'll manage the property of
     * @param propertyName The name of the property in @p object which we'll manage
     * @param defaultValue The default value of the property
     */
    KPropertySkeletonItem(QObject *object, const QByteArray &propertyName, const QVariant &defaultValue);

    /** @copydoc KConfigSkeletonItem::property() */
    QVariant property() const override;
    /** @copydoc KConfigSkeletonItem::setProperty(const QVariant &) */
    void setProperty(const QVariant &p) override;
    /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) */
    bool isEqual(const QVariant &p) const override;

    /** @copydoc KConfigSkeletonItem::readConfig(KConfig *) */
    void readConfig(KConfig *) override;
    /** @copydoc KConfigSkeletonItem::writeConfig(KConfig *) */
    void writeConfig(KConfig *) override;

    /** @copydoc KConfigSkeletonItem::readDefault(KConfig *) */
    void readDefault(KConfig *) override;
    /** @copydoc KConfigSkeletonItem::setDefault() */
    void setDefault() override;
    /** @copydoc KConfigSkeletonItem::swapDefault() */
    void swapDefault() override;

    /**
     * Set a notify function, it will be invoked when the value of the property changes.
     * @since 5.68
     */
    void setNotifyFunction(const std::function<void()> &impl);
};

/**
 * \class KConfigSkeletonGenericItem kcoreconfigskeleton.h <KCoreConfigSkeleton>
 *
 * @short Base class for storing a preferences setting of type @p T.
 */
template<typename T>
class KConfigSkeletonGenericItem : public KConfigSkeletonItem
{
public:
    /**
     * @copydoc KConfigSkeletonItem(const QString&, const QString&)
     * @param reference The initial value to hold in the item
     * @param defaultValue The default value for the item
     */
    KConfigSkeletonGenericItem(const QString &_group, const QString &_key, T &reference, T defaultValue)
        : KConfigSkeletonItem(_group, _key)
        , mReference(reference)
        , mDefault(defaultValue)
        , mLoadedValue(defaultValue)
    {
        setIsDefaultImpl([this] {
            return mReference == mDefault;
        });
        setIsSaveNeededImpl([this] {
            return mReference != mLoadedValue;
        });
        setGetDefaultImpl([this] {
            return QVariant::fromValue(mDefault);
        });
    }

    /**
     * Set value of this KConfigSkeletonItem.
     */
    void setValue(const T &v)
    {
        mReference = v;
    }

    /**
     * Return value of this KConfigSkeletonItem.
     */
    T &value()
    {
        return mReference;
    }

    /**
     * Return const value of this KConfigSkeletonItem.
     */
    const T &value() const
    {
        return mReference;
    }

    /**
     * Set default value for this item.
     */
    virtual void setDefaultValue(const T &v)
    {
        mDefault = v;
    }

    /**
     * Set the value for this item to the default value
     */
    void setDefault() override
    {
        mReference = mDefault;
    }

    /** @copydoc KConfigSkeletonItem::writeConfig(KConfig *) */
    void writeConfig(KConfig *config) override
    {
        if (mReference != mLoadedValue) { // Is this needed?
            KConfigGroup cg = configGroup(config);
            if ((mDefault == mReference) && !cg.hasDefault(mKey)) {
                cg.revertToDefault(mKey, writeFlags());
            } else {
                cg.writeEntry(mKey, mReference, writeFlags());
            }
            mLoadedValue = mReference;
        }
    }

    /** @copydoc KConfigSkeletonItem::readDefault(KConfig*) */
    void readDefault(KConfig *config) override
    {
        config->setReadDefaults(true);
        readConfig(config);
        config->setReadDefaults(false);
        mDefault = mReference;
    }

    /** @copydoc KConfigSkeletonItem::swapDefault() */
    void swapDefault() override
    {
        T tmp = mReference;
        mReference = mDefault;
        mDefault = tmp;
    }

protected:
    T &mReference; ///< Stores the value for this item
    T mDefault; ///< The default value for this item
    T mLoadedValue;
};

/**
 * \class KConfigCompilerSignallingItem kcoreconfigskeleton.h <KCoreConfigSkeleton>
 *
 * @author Alex Richardson
 *
 * This class wraps a @ref KConfigSkeletonItem and invokes a function whenever the value changes.
 * That function must take one quint64 parameter. Whenever the property value of the wrapped KConfigSkeletonItem
 * changes this function will be invoked with the stored user data passed in the constructor.
 * It does not call a function with the new value since this class is designed solely for the \ref kconfig_compiler generated
 * code and is therefore probably not suited for any other usecases.
 *
 * @see KConfigSkeletonItem
 */
class KCONFIGCORE_EXPORT KConfigCompilerSignallingItem : public KConfigSkeletonItem
{
public:
    typedef void (QObject::*NotifyFunction)(quint64 arg);
    /**
     * Constructor.
     *
     * @param item the KConfigSkeletonItem to wrap
     * @param targetFunction the method to invoke whenever the value of @p item changes
     * @param object The object on which the method is invoked.
     * @param userData This data will be passed to @p targetFunction on every property change
     */
    KConfigCompilerSignallingItem(KConfigSkeletonItem *item, QObject *object, NotifyFunction targetFunction, quint64 userData);
    ~KConfigCompilerSignallingItem() override;

    void readConfig(KConfig *) override;
    void writeConfig(KConfig *) override;
    void readDefault(KConfig *) override;
    void setProperty(const QVariant &p) override;
    bool isEqual(const QVariant &p) const override;
    QVariant property() const override;
    QVariant minValue() const override;
    QVariant maxValue() const override;
    void setDefault() override;
    void swapDefault() override;
    // KF6 TODO - fix this
    // Ideally we would do this in an overload of KConfigSkeletonItem, but
    // given we can't, I've shadowed the method. This isn't pretty, but given
    // the docs say it should generally only be used from auto generated code,
    // should be fine.
    void setWriteFlags(KConfigBase::WriteConfigFlags flags);
    KConfigBase::WriteConfigFlags writeFlags() const;
    void setGroup(const KConfigGroup &cg);
    KConfigGroup configGroup(KConfig *config) const;
    // END TODO

private:
    inline void invokeNotifyFunction()
    {
        // call the pointer to member function using the strange ->* operator
        (mObject->*mTargetFunction)(mUserData);
    }

private:
    QScopedPointer<KConfigSkeletonItem> mItem;
    NotifyFunction mTargetFunction;
    QObject *mObject;
    quint64 mUserData;
};

/**
 * \class KCoreConfigSkeleton kcoreconfigskeleton.h <KCoreConfigSkeleton>
 *
 * @short Class for handling preferences settings for an application.
 * @author Cornelius Schumacher
 *
 * This class provides an interface to preferences settings. Preferences items
 * can be registered by the addItem() function corresponding to the data type of
 * the setting. KCoreConfigSkeleton then handles reading and writing of config files and
 * setting of default values.
 *
 * Normally you will subclass KCoreConfigSkeleton, add data members for the preferences
 * settings and register the members in the constructor of the subclass.
 *
 * Example:
 * \code
 * class MyPrefs : public KCoreConfigSkeleton
 * {
 *   public:
 *     MyPrefs()
 *     {
 *       setCurrentGroup("MyGroup");
 *       addItemBool("MySetting1", mMyBool, false);
 *       addItemPoint("MySetting2", mMyPoint, QPoint(100, 200));
 *
 *       setCurrentGroup("MyOtherGroup");
 *       addItemDouble("MySetting3", mMyDouble, 3.14);
 *     }
 *
 *     bool mMyBool;
 *     QPoint mMyPoint;
 *     double mMyDouble;
 * }
 * \endcode
 *
 * It might be convenient in many cases to make this subclass of KCoreConfigSkeleton a
 * singleton for global access from all over the application without passing
 * references to the KCoreConfigSkeleton object around.
 *
 * You can write the data to the configuration file by calling @ref save()
 * and read the data from the configuration file by calling @ref readConfig().
 * If you want to watch for config changes, use @ref configChanged() signal.
 *
 * If you have items, which are not covered by the existing addItem() functions
 * you can add customized code for reading, writing and default setting by
 * implementing the functions @ref usrUseDefaults(), @ref usrRead() and
 * @ref usrSave().
 *
 * Internally preferences settings are stored in instances of subclasses of
 * @ref KConfigSkeletonItem. You can also add KConfigSkeletonItem subclasses
 * for your own types and call the generic @ref addItem() to register them.
 *
 * In many cases you don't have to write the specific KCoreConfigSkeleton
 * subclasses yourself, but you can use \ref kconfig_compiler to automatically
 * generate the C++ code from an XML description of the configuration options.
 *
 * Use KConfigSkeleton if you need GUI types as well.
 *
 * @see KConfigSkeletonItem
 */
class KCONFIGCORE_EXPORT KCoreConfigSkeleton : public QObject
{
    Q_OBJECT
public:
    /**
     * Class for handling a string preferences item.
     */
    class KCONFIGCORE_EXPORT ItemString : public KConfigSkeletonGenericItem<QString>
    {
    public:
        /** The type of string that is held in this item */
        enum Type {
            Normal, ///< A normal string
            Password, ///< A password string
            Path, ///< A path to a file or directory
        };

        /**
         * @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem
         * @param type The type of string held by the item
         */
        ItemString(const QString &_group,
                   const QString &_key,
                   QString &reference,
                   const QString &defaultValue = QLatin1String(""), // NOT QString() !!
                   Type type = Normal);

        /** @copydoc KConfigSkeletonItem::writeConfig(KConfig*) */
        void writeConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::setProperty(const QVariant&) */
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() const */
        QVariant property() const override;

    private:
        Type mType;
    };

    /**
     * Class for handling a password preferences item.
     */
    class KCONFIGCORE_EXPORT ItemPassword : public ItemString
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemPassword(const QString &_group, const QString &_key, QString &reference,
                     const QString &defaultValue = QLatin1String("")); // NOT QString() !!
    };

    /**
     * Class for handling a path preferences item.
     */
    class KCONFIGCORE_EXPORT ItemPath : public ItemString
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemPath(const QString &_group, const QString &_key, QString &reference, const QString &defaultValue = QString());
    };

    /**
     * Class for handling a url preferences item.
     */
    class KCONFIGCORE_EXPORT ItemUrl : public KConfigSkeletonGenericItem<QUrl>
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemUrl(const QString &_group, const QString &_key, QUrl &reference, const QUrl &defaultValue = QUrl());

        /** @copydoc KConfigSkeletonItem::writeConfig(KConfig*) */
        void writeConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::setProperty(const QVariant&) */
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() const */
        QVariant property() const override;
    };

    /**
     * Class for handling a QVariant preferences item.
     */
    class KCONFIGCORE_EXPORT ItemProperty : public KConfigSkeletonGenericItem<QVariant>
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemProperty(const QString &_group, const QString &_key, QVariant &reference, const QVariant &defaultValue = QVariant());

        void readConfig(KConfig *config) override;
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() const */
        QVariant property() const override;
    };

    /**
     * Class for handling a bool preferences item.
     */
    class KCONFIGCORE_EXPORT ItemBool : public KConfigSkeletonGenericItem<bool>
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemBool(const QString &_group, const QString &_key, bool &reference, bool defaultValue = true);

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::setProperty(const QVariant&) */
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() const */
        QVariant property() const override;
    };

    /**
     * Class for handling a 32-bit integer preferences item.
     */
    class KCONFIGCORE_EXPORT ItemInt : public KConfigSkeletonGenericItem<qint32>
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemInt(const QString &_group, const QString &_key, qint32 &reference, qint32 defaultValue = 0);

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::setProperty(const QVariant&) */
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() */
        QVariant property() const override;

        /** Get the minimum value that is allowed to be stored in this item */
        QVariant minValue() const override;

        /** Get the maximum value this is allowed to be stored in this item */
        QVariant maxValue() const override;

        /**
         * Set the minimum value for the item.
         * @see minValue()
         */
        void setMinValue(qint32);

        /**
         * Set the maximum value for the item.
         * @see maxValue
         */
        void setMaxValue(qint32);

    private:
        bool mHasMin : 1;
        bool mHasMax : 1;
        qint32 mMin;
        qint32 mMax;
    };

    /**
     * Class for handling a 64-bit integer preferences item.
     */
    class KCONFIGCORE_EXPORT ItemLongLong : public KConfigSkeletonGenericItem<qint64>
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemLongLong(const QString &_group, const QString &_key, qint64 &reference, qint64 defaultValue = 0);

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::setProperty(const QVariant&) */
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() */
        QVariant property() const override;

        /** @copydoc ItemInt::minValue() */
        QVariant minValue() const override;

        /** @copydoc ItemInt::maxValue() */
        QVariant maxValue() const override;

        /** @copydoc ItemInt::setMinValue(qint32) */
        void setMinValue(qint64);

        /** @copydoc ItemInt::setMaxValue(qint32) */
        void setMaxValue(qint64);

    private:
        bool mHasMin : 1;
        bool mHasMax : 1;
        qint64 mMin;
        qint64 mMax;
    };

    /**
     * Class for handling enums.
     */
    class KCONFIGCORE_EXPORT ItemEnum : public ItemInt
    {
    public:
        struct Choice {
            QString name;
            QString label;
            QString toolTip;
            QString whatsThis;
        };

        /**
         * @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem
         * @param choices The list of enums that can be stored in this item
         */
        ItemEnum(const QString &_group, const QString &_key, qint32 &reference, const QList<Choice> &choices, qint32 defaultValue = 0);

        QList<Choice> choices() const;

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::writeConfig(KConfig*) */
        void writeConfig(KConfig *config) override;

        /**
         * Returns the value for the choice with the given @p name
         */
        QString valueForChoice(const QString &name) const;

        /**
         * Stores a choice value for @p name
         */
        void setValueForChoice(const QString &name, const QString &valueForChoice);

    private:
        QList<Choice> mChoices;
    };

    /**
     * Class for handling an unsigned 32-bit integer preferences item.
     */
    class KCONFIGCORE_EXPORT ItemUInt : public KConfigSkeletonGenericItem<quint32>
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemUInt(const QString &_group, const QString &_key, quint32 &reference, quint32 defaultValue = 0);

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::setProperty(const QVariant&) */
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() */
        QVariant property() const override;

        /** @copydoc ItemInt::minValue() */
        QVariant minValue() const override;

        /** @copydoc ItemInt::maxValue() */
        QVariant maxValue() const override;

        /** @copydoc ItemInt::setMinValue(qint32) */
        void setMinValue(quint32);

        /** @copydoc ItemInt::setMaxValue(qint32) */
        void setMaxValue(quint32);

    private:
        bool mHasMin : 1;
        bool mHasMax : 1;
        quint32 mMin;
        quint32 mMax;
    };

    /**
     * Class for handling unsigned 64-bit integer preferences item.
     */
    class KCONFIGCORE_EXPORT ItemULongLong : public KConfigSkeletonGenericItem<quint64>
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemULongLong(const QString &_group, const QString &_key, quint64 &reference, quint64 defaultValue = 0);

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::setProperty(const QVariant&) */
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() */
        QVariant property() const override;

        /** @copydoc ItemInt::minValue() */
        QVariant minValue() const override;

        /** @copydoc ItemInt::maxValue() */
        QVariant maxValue() const override;

        /** @copydoc ItemInt::setMinValue(qint32) */
        void setMinValue(quint64);

        /** @copydoc ItemInt::setMaxValue(qint32) */
        void setMaxValue(quint64);

    private:
        bool mHasMin : 1;
        bool mHasMax : 1;
        quint64 mMin;
        quint64 mMax;
    };

    /**
     * Class for handling a floating point preference item.
     */
    class KCONFIGCORE_EXPORT ItemDouble : public KConfigSkeletonGenericItem<double>
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemDouble(const QString &_group, const QString &_key, double &reference, double defaultValue = 0);

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::setProperty(const QVariant&) */
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() */
        QVariant property() const override;

        /** @copydoc ItemInt::minValue() */
        QVariant minValue() const override;

        /** @copydoc ItemInt::maxValue() */
        QVariant maxValue() const override;

        /** @copydoc ItemInt::setMinValue() */
        void setMinValue(double);

        /** @copydoc ItemInt::setMaxValue() */
        void setMaxValue(double);

    private:
        bool mHasMin : 1;
        bool mHasMax : 1;
        double mMin;
        double mMax;
    };

    /**
     * Class for handling a QRect preferences item.
     */
    class KCONFIGCORE_EXPORT ItemRect : public KConfigSkeletonGenericItem<QRect>
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemRect(const QString &_group, const QString &_key, QRect &reference, const QRect &defaultValue = QRect());

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::setProperty(const QVariant&) */
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() */
        QVariant property() const override;
    };

    /**
     * Class for handling a QPoint preferences item.
     */
    class KCONFIGCORE_EXPORT ItemPoint : public KConfigSkeletonGenericItem<QPoint>
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemPoint(const QString &_group, const QString &_key, QPoint &reference, const QPoint &defaultValue = QPoint());

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::setProperty(const QVariant&) */
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() */
        QVariant property() const override;
    };

    /**
     * Class for handling a QSize preferences item.
     */
    class KCONFIGCORE_EXPORT ItemSize : public KConfigSkeletonGenericItem<QSize>
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemSize(const QString &_group, const QString &_key, QSize &reference, const QSize &defaultValue = QSize());

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::setProperty(const QVariant&) */
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() */
        QVariant property() const override;
    };

    /**
     * Class for handling a QDateTime preferences item.
     */
    class KCONFIGCORE_EXPORT ItemDateTime : public KConfigSkeletonGenericItem<QDateTime>
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemDateTime(const QString &_group, const QString &_key, QDateTime &reference, const QDateTime &defaultValue = QDateTime());

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::setProperty(const QVariant&) */
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() */
        QVariant property() const override;
    };

    /**
     * Class for handling a string list preferences item.
     */
    class KCONFIGCORE_EXPORT ItemStringList : public KConfigSkeletonGenericItem<QStringList>
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemStringList(const QString &_group, const QString &_key, QStringList &reference, const QStringList &defaultValue = QStringList());

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::setProperty(const QVariant&) */
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() */
        QVariant property() const override;
    };

    /**
     * Class for handling a path list preferences item.
     */
    class KCONFIGCORE_EXPORT ItemPathList : public ItemStringList
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemPathList(const QString &_group, const QString &_key, QStringList &reference, const QStringList &defaultValue = QStringList());

        /** @copydoc KConfigSkeletonItem::readConfig */
        void readConfig(KConfig *config) override;
        /** @copydoc KConfigSkeletonItem::writeConfig */
        void writeConfig(KConfig *config) override;
    };

    /**
     * Class for handling a url list preferences item.
     */
    class KCONFIGCORE_EXPORT ItemUrlList : public KConfigSkeletonGenericItem<QList<QUrl>>
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemUrlList(const QString &_group, const QString &_key, QList<QUrl> &reference, const QList<QUrl> &defaultValue = QList<QUrl>());

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::writeConfig(KConfig*) */
        void writeConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::setProperty(const QVariant&) */
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() */
        QVariant property() const override;
    };

    /**
     * Class for handling an integer list preferences item.
     */
    class KCONFIGCORE_EXPORT ItemIntList : public KConfigSkeletonGenericItem<QList<int>>
    {
    public:
        /** @copydoc KConfigSkeletonGenericItem::KConfigSkeletonGenericItem */
        ItemIntList(const QString &_group, const QString &_key, QList<int> &reference, const QList<int> &defaultValue = QList<int>());

        /** @copydoc KConfigSkeletonItem::readConfig(KConfig*) */
        void readConfig(KConfig *config) override;

        /** @copydoc KConfigSkeletonItem::setProperty(const QVariant&) */
        void setProperty(const QVariant &p) override;

        /** @copydoc KConfigSkeletonItem::isEqual(const QVariant &) const */
        bool isEqual(const QVariant &p) const override;

        /** @copydoc KConfigSkeletonItem::property() */
        QVariant property() const override;
    };

public:
    /**
     * Constructor.
     *
     * @param assoiation config association - is it a kde application, a plasma component, or neither?
     *                   This will determine where the config file is saved, if the path given in name
     *                   is empty or relative. It is ignored for absolute path.
     * @param configname name of config file. If no name is given, the default
     *                   config file as returned by KSharedConfig::openConfig() is used
     * @param parent the parent object (see QObject documentation)
     */
    explicit KCoreConfigSkeleton(KConfig::ConfigAssociation association, const QString &configname = QString(), QObject *parent = nullptr);

    /**
     * This constructor is deprecated
     */
    [[deprecated(
        "This constructor is deprecated, please specify association by calling KCoreConfigSkeleton(KConfig::ConfigAssociation association, const QString "
        "&configname = QString(), QObject *parent = nullptr)")]] explicit KCoreConfigSkeleton(const QString &configname = QString(), QObject *parent = nullptr);

    /**
     * Constructor.
     *
     * @param config configuration object to use
     * @param parent the parent object (see QObject documentation)
     */
    explicit KCoreConfigSkeleton(KSharedConfig::Ptr config, QObject *parent = nullptr);

    /**
     * Destructor
     */
    ~KCoreConfigSkeleton() override;

    /**
     * Set all registered items to their default values.
     * This method calls usrSetDefaults() after setting the defaults for the
     * registered items. You can override usrSetDefaults() in derived classes
     * if you have special requirements.
     * If you need more fine-grained control of setting the default values of
     * the registered items you can override setDefaults() in a derived class.
     */
    virtual void setDefaults();

    /**
     * Read preferences from config file. All registered items are set to the
     * values read from disk.
     * This method calls usrRead() after reading the settings of the
     * registered items from the KConfig. You can override usrRead()
     * in derived classes if you have special requirements.
     */
    void load();

    /**
     * Read preferences from the KConfig object.
     * This method assumes that the KConfig object was previously loaded,
     * i.e. it uses the in-memory values from KConfig without reloading from disk.
     *
     * This method calls usrRead() after reading the settings of the
     * registered items from the KConfig. You can override usrRead()
     * in derived classes if you have special requirements.
     * @since 5.0
     */
    void read();

    /**
     * Indicates if all the registered items are set to their default value.
     *
     * @since 5.64
     */
    bool isDefaults() const;

    /**
     * Indicates if any registered item has a different value than the
     * previously loaded value.
     *
     * @since 5.64
     */
    bool isSaveNeeded() const;

    /**
     * Set the config file group for subsequent addItem() calls. It is valid
     * until setCurrentGroup() is called with a new argument. Call this before
     * you add any items. The default value is "No Group".
     */
    void setCurrentGroup(const QString &group);

    /**
     * Returns the current group used for addItem() calls.
     */
    QString currentGroup() const;

    /**
     * Register a custom @ref KConfigSkeletonItem @p item with a given @p name.
     *
     * If @p name is a null string, take the name from KConfigSkeletonItem::key().
     *
     * @note All names must be unique but multiple entries can have
     * the same key if they reside in different groups.
     *
     * KCoreConfigSkeleton takes ownership of @p item.
     */
    void addItem(KConfigSkeletonItem *item, const QString &name = QString());

    /**
     * Register an item of type QString.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemString *addItemString(const QString &name,
                              QString &reference,
                              const QString &defaultValue = QLatin1String(""), // NOT QString() !!
                              const QString &key = QString());

    /**
     * Register a password item of type QString. The string value is written
     * encrypted to the config file.
     *
     * @note The current encryption scheme is very weak.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemPassword *addItemPassword(const QString &name, QString &reference, const QString &defaultValue = QLatin1String(""), const QString &key = QString());

    /**
     * Register a path item of type QString. The string value is interpreted
     * as a path. This means, dollar expansion is activated for this value, so
     * that e.g. @c $HOME gets expanded.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemPath *addItemPath(const QString &name, QString &reference, const QString &defaultValue = QLatin1String(""), const QString &key = QString());

    /**
     * Register a property item of type QVariant.
     *
     * @note The following QVariant types are allowed:
     * String, StringList, Font, Point, Rect, Size,
     * Color, Int, UInt, Bool, Double, DateTime and Date.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemProperty *addItemProperty(const QString &name, QVariant &reference, const QVariant &defaultValue = QVariant(), const QString &key = QString());
    /**
     * Register an item of type @c bool.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemBool *addItemBool(const QString &name, bool &reference, bool defaultValue = false, const QString &key = QString());

    /**
     * Register an item of type @c qint32.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemInt *addItemInt(const QString &name, qint32 &reference, qint32 defaultValue = 0, const QString &key = QString());

    /**
     * Register an item of type @c quint32.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemUInt *addItemUInt(const QString &name, quint32 &reference, quint32 defaultValue = 0, const QString &key = QString());

    /**
     * Register an item of type @c qint64.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemLongLong *addItemLongLong(const QString &name, qint64 &reference, qint64 defaultValue = 0, const QString &key = QString());

    /**
     * Register an item of type @c quint64.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemULongLong *addItemULongLong(const QString &name, quint64 &reference, quint64 defaultValue = 0, const QString &key = QString());

    /**
     * Register an item of type @c double.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemDouble *addItemDouble(const QString &name, double &reference, double defaultValue = 0.0, const QString &key = QString());

    /**
     * Register an item of type QRect.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemRect *addItemRect(const QString &name, QRect &reference, const QRect &defaultValue = QRect(), const QString &key = QString());

    /**
     * Register an item of type QPoint.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemPoint *addItemPoint(const QString &name, QPoint &reference, const QPoint &defaultValue = QPoint(), const QString &key = QString());

    /**
     * Register an item of type QSize.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemSize *addItemSize(const QString &name, QSize &reference, const QSize &defaultValue = QSize(), const QString &key = QString());

    /**
     * Register an item of type QDateTime.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemDateTime *addItemDateTime(const QString &name, QDateTime &reference, const QDateTime &defaultValue = QDateTime(), const QString &key = QString());

    /**
     * Register an item of type QStringList.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemStringList *
    addItemStringList(const QString &name, QStringList &reference, const QStringList &defaultValue = QStringList(), const QString &key = QString());

    /**
     * Register an item of type QList<int>.
     *
     * @param name Name used to identify this setting. Names must be unique.
     * @param reference Pointer to the variable, which is set by readConfig()
     * calls and read by save() calls.
     * @param defaultValue Default value, which is used when the config file
     * does not yet contain the key of this item.
     * @param key Key used in config file. If @p key is a null string, @p name is used as key.
     * @return The created item
     */
    ItemIntList *addItemIntList(const QString &name, QList<int> &reference, const QList<int> &defaultValue = QList<int>(), const QString &key = QString());

    /**
     * Return the @ref KConfig object used for reading and writing the settings.
     */
    KConfig *config();

    /**
     * Return the @ref KConfig object used for reading and writing the settings.
     */
    const KConfig *config() const;

    /**
     * Return the @ref KConfig object used for reading and writing the settings.
     * @since 5.0
     */
    KSharedConfig::Ptr sharedConfig() const;

    /**
     * Set the @ref KSharedConfig object used for reading and writing the settings.
     */
    void setSharedConfig(KSharedConfig::Ptr pConfig);

    /**
     * Return list of items managed by this KCoreConfigSkeleton object.
     */
    KConfigSkeletonItem::List items() const;

    /**
     * Removes and deletes an item by name
     * @param name the name of the item to remove
     */
    void removeItem(const QString &name);

    /**
     * Removes and deletes all items
     */
    void clearItems();

    /**
     * Return whether a certain item is immutable
     * @since 4.4
     */
    Q_INVOKABLE bool isImmutable(const QString &name) const;

    /**
     * Lookup item by name
     * @since 4.4
     */
    KConfigSkeletonItem *findItem(const QString &name) const;

    /**
     * Specify whether this object should reflect the actual values or the
     * default values.
     * This method is implemented by usrUseDefaults(), which can be overridden
     * in derived classes if you have special requirements and can call
     * usrUseDefaults() directly.
     * If you don't have control whether useDefaults() or usrUseDefaults() is
     * called override useDefaults() directly.
     * @param b @c true to make this object reflect the default values,
     *          @c false to make it reflect the actual values.
     * @return The state prior to this call
     */
    virtual bool useDefaults(bool b);

public Q_SLOTS:
    /**
     * Write preferences to config file. The values of all registered items are
     * written to disk.
     * This method calls usrSave() after writing the settings from the
     * registered items to the KConfig. You can override usrSave()
     * in derived classes if you have special requirements.
     */
    bool save();

Q_SIGNALS:
    /**
     * This signal is emitted when the configuration change.
     */
    void configChanged();

protected:
    /**
     * Implemented by subclasses that use special defaults.
     * It replaces the default values with the actual values and
     * vice versa.  Called from @ref useDefaults()
     * @param b @c true to make this object reflect the default values,
     *          @c false to make it reflect the actual values.
     * @return The state prior to this call
     */
    virtual bool usrUseDefaults(bool b);

    /**
     * Perform the actual setting of default values.
     * Override in derived classes to set special default values.
     * Called from @ref setDefaults()
     */
    virtual void usrSetDefaults();

    /**
     * Perform the actual reading of the configuration file.
     * Override in derived classes to read special config values.
     * Called from @ref read()
     */
    virtual void usrRead();

    /**
     * Perform the actual writing of the configuration file.
     * Override in derived classes to write special config values.
     * Called from @ref save()
     */
    virtual bool usrSave();

private:
    KCoreConfigSkeletonPrivate *const d;
    friend class KConfigSkeleton;
};

#endif
