#pragma once

#include <Parsers/IAST.h>

namespace DB
{
class IASTColumnsTransformer : public IAST
{
public:
    virtual void transform(ASTs & nodes) const = 0;
    static void transform(const ASTPtr & transformer, ASTs & nodes);
};

class ASTColumnsApplyTransformer : public IASTColumnsTransformer
{
public:
    String getID(char) const override { return "ColumnsApplyTransformer"; }
    ASTPtr clone() const override
    {
        auto res = std::make_shared<ASTColumnsApplyTransformer>(*this);
        return res;
    }
    void transform(ASTs & nodes) const override;
    String func_name;

protected:
    void formatImpl(const FormatSettings & settings, FormatState &, FormatStateStacked) const override;
};

class ASTColumnsExceptTransformer : public IASTColumnsTransformer
{
public:
    String getID(char) const override { return "ColumnsExceptTransformer"; }
    ASTPtr clone() const override
    {
        auto clone = std::make_shared<ASTColumnsExceptTransformer>(*this);
        clone->cloneChildren();
        return clone;
    }
    void transform(ASTs & nodes) const override;

protected:
    void formatImpl(const FormatSettings & settings, FormatState &, FormatStateStacked) const override;
};

class ASTColumnsReplaceTransformer : public IASTColumnsTransformer
{
public:
    class Replacement : public IAST
    {
    public:
        String getID(char) const override { return "ColumnsReplaceTransformer::Replacement"; }
        ASTPtr clone() const override
        {
            auto replacement = std::make_shared<Replacement>(*this);
            replacement->children.clear();
            replacement->expr = expr->clone();
            replacement->children.push_back(replacement->expr);
            return replacement;
        }

        String name;
        ASTPtr expr;

    protected:
        void formatImpl(const FormatSettings & settings, FormatState &, FormatStateStacked) const override;
    };

    String getID(char) const override { return "ColumnsReplaceTransformer"; }
    ASTPtr clone() const override
    {
        auto clone = std::make_shared<ASTColumnsReplaceTransformer>(*this);
        clone->cloneChildren();
        return clone;
    }
    void transform(ASTs & nodes) const override;

protected:
    void formatImpl(const FormatSettings & settings, FormatState &, FormatStateStacked) const override;

private:
    static void replaceChildren(ASTPtr & node, const ASTPtr & replacement, const String & name);
};

}
